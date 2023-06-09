/*
 * InterCommsModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

//#include "PiCommsDriver.h"
#include "InterCommsModule.h"
#include "DataAggregationModule.h"
#include "PiCommsDriver.h"
#include "MovementControlModule.h"
#include "AppendageActuationModule.h"

#include "..\..\..\Middlewares\Third_Party\NanoPB\pb_decode.h"
#include "..\..\..\Middlewares\Third_Party\NanoPB\pb_encode.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UserTypes.h"


const char * TAG = "ICM";

#define RX_BUFFER_SIZE 128		//size of the largest possible rx message
static uint8_t piComms_rxBuffer[RX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions

static uint8_t rxByte = 0;		//pointer to where in the rxBuffer the next character will go
static uint8_t rxLen;
static bool gotLen;

static uint16_t rxPtrOffset;

static bool msgEndMode = false;	//true if we are in message end mode

static uint8_t delimiter = 0b0;
static uint8_t delimCtr;


PRIVATE void SetPollMessageValues(KR23_PollMessage pollMessage);
PRIVATE void HandlePiCommsMessage(PiOutgoingMessage_t msg);
PRIVATE void PrintPiCommsMessage(PiOutgoingMessage_t msg);
PRIVATE void PiComms_handleMessage(uint16_t protobufLen);

PUBLIC void InitInternalCommsModule(){
	PiComms_Init();
}


void clearRx()
{
	gotLen = false;
	memset(piComms_rxBuffer, 0, RX_BUFFER_SIZE);
	rxPtrOffset = 0;
}

static uint8_t counter = 0;
PUBLIC void ProcessRxTransmission()
{


    rxByte = PiComms_DequeueByte();
    // message end mode: if we're not in sync with the delim'd frames, clear the current RX values and buffer and find the next delim'd frame

	if (msgEndMode)
	{
		uint8_t tmp = 1;
		while (delimCtr < 2)
		{
			tmp = PiComms_DequeueByte();
			if (tmp == delimiter)
			{
				delimCtr++;
			}
			else
			{
				delimCtr = 0;
			}
		}
		clearRx();
		msgEndMode = false;
		counter = 0;
		return;
	}

	// if we're not in message end mode, we're in regular message mode, so we move on to regular operations

	// first check if we're outside the bounds of the rxBuffer
	if (rxPtrOffset >= RX_BUFFER_SIZE)
	{
		SerialDebug(TAG, "rxPtrOffset >= RX_BUFFER_SIZE");
		clearRx();
		msgEndMode = true;
		return;
	}

	// then check if we've received the length of the protobuf message
	if (!gotLen)
	{
		rxLen = rxByte;
		gotLen = true;
		return;
	}

	// then check if the length of the protobuf message is within bounds
	if (rxLen > RX_BUFFER_SIZE)
	{
		SerialDebug(TAG, "rxLen > RX_BUFFER_SIZE");
		clearRx();
		msgEndMode = true;
		return;
	}

	// then while the buffer is not full, receive the protobuf message
	if (rxPtrOffset < rxLen)
	{
		piComms_rxBuffer[rxPtrOffset] = rxByte;
		// then check if the byte we just received is a delimiter
		if (rxByte == delimiter)
		{
			delimCtr++;
		}
		else
		{
			delimCtr = 0;
		}
		// then check if we've received the delimiter twice in a row; if so, we drop the message and re-start
		if (delimCtr == 2)
		{
			SerialDebug(TAG, "delimCtr == 2 in rxBuffer, dropping message");
			clearRx();
			return;
		}
		rxPtrOffset++;
		return;
	}

	// then check if the next two bytes are the delimiter, using a blocking receive, and if so, we've received the full message
    uint8_t tmp_1 = rxByte;
    uint8_t tmp_2 = 3;
    tmp_2 = PiComms_DequeueByte();
    if (tmp_1 == delimiter && tmp_2 == delimiter)
    {
        // we've received the full message, so we can handle it
        PiComms_handleMessage(rxLen);
        clearRx();
        return;
    }
	// if we've gotten here, we've received the full message but it's not delimited properly, so we drop the message and re-start
	SerialDebug(TAG, "message not delimited properly, dropping message, delims = %d %d", tmp_1, tmp_2);
	clearRx();
	msgEndMode = true;
	PiComms_ResetByteQueue();
	PiCommsUARTAbort();

}

/*Allocates and assigns  */
PRIVATE void PiComms_handleMessage(uint16_t protobufLen)
{
	const pb_byte_t * pbBuffer = (pb_byte_t *)piComms_rxBuffer;
	KR23_OutgoingMessage recievedMessage;

	pb_istream_t istream = pb_istream_from_buffer(pbBuffer, protobufLen * sizeof(uint8_t));
	bool decodeSuccessful = pb_decode(&istream, KR23_OutgoingMessage_fields, &recievedMessage);
	if(decodeSuccessful){
		HandlePiCommsMessage(recievedMessage);

	}else{
		SerialDebug(TAG, "Problem decoding Message");
	}
}


PRIVATE void HandlePiCommsMessage(PiOutgoingMessage_t msg){
	//PrintPiCommsMessage(msg);
	if(msg.has_attachmentCommand){
		AAMod_Callback(msg.attachmentCommand);
	}
	if(msg.has_thrusterCommand){
		MCMod_ThrusterCallback(msg.thrusterCommand);
	}

	KR23_IncomingMessage imPoll = KR23_IncomingMessage_init_zero;
	imPoll.has_pollMessage = true;
	SetPollMessageValues(imPoll.pollMessage);

	PiComms_Send(imPoll);
}


//sets the poll message values
PRIVATE void SetPollMessageValues(KR23_PollMessage pollMessage){
	pollMessage.claw_state = DA_GetClawState();
	pollMessage.light_PWM = DA_GetLightValue();
	pollMessage.servo_0_PWM = DA_GetPWMServoValue(0);
	pollMessage.thruster_0_PWM = DA_GetThrusterValue(0);
	pollMessage.result = KR23_KelpieResult_OK;
}


//Helper function for  debugging. Will need to be updated when .pb.c/h files are generated just like callbacks and HandlePiCommsMessage_t
PRIVATE void PrintPiCommsMessage(PiOutgoingMessage_t msg){
	SerialPrintln("PrintPiCommsMessage_t");
	if(msg.has_attachmentCommand){
		SerialPrintln("attachmentCommand");
		SerialPrint("claw_state: %d\t", msg.attachmentCommand.claw_state);
		SerialPrint("light_PWM: %d\t", msg.attachmentCommand.light_PWM);
		SerialPrint("servo_0_PWM: %d\t", msg.attachmentCommand.servo_0_PWM);
		SerialPrint("servo_1_PWM: %d\t", msg.attachmentCommand.servo_1_PWM);
		SerialPrint("servo_2_PWM: %d\t", msg.attachmentCommand.servo_2_PWM);
		SerialPrint("result: %d\t", msg.attachmentCommand.result);
	}
	SerialPrintln("");
	if(msg.has_thrusterCommand){
		SerialPrintln("thrusterCommand");
		SerialPrint("thruster_0_PWM: %d\t", msg.thrusterCommand.thruster_0_PWM);
		SerialPrint("thruster_1_PWM: %d\t", msg.thrusterCommand.thruster_1_PWM);
		SerialPrint("thruster_2_PWM: %d\t", msg.thrusterCommand.thruster_2_PWM);
		SerialPrintln("thruster_3_PWM: %d\t", msg.thrusterCommand.thruster_3_PWM);
		SerialPrint("thruster_4_PWM: %d\t", msg.thrusterCommand.thruster_4_PWM);
		SerialPrint("thruster_5_PWM: %d\t", msg.thrusterCommand.thruster_5_PWM);
		SerialPrint("thruster_6_PWM: %d\t", msg.thrusterCommand.thruster_6_PWM);
		SerialPrintln("thruster_7_PWM: %d\t", msg.thrusterCommand.thruster_7_PWM);
		SerialPrint("result: %d\t", msg.thrusterCommand.result);
	}
	SerialPrintln("");
}


