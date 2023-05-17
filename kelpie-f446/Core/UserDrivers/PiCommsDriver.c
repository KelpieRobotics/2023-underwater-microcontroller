/*
 * PiCommsDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingye eric
 */

#include "PiCommsDriver.h"
#include "SerialDebugDriver.h"
#include "UserTypes.h"
#include "..\..\..\Middlewares\Third_Party\NanoPB\pb_decode.h"
#include "..\..\..\Middlewares\Third_Party\NanoPB\pb_encode.h"

#include "stdlib.h"

#define TAG "PCM"

extern UART_HandleTypeDef huart4;
UART_HandleTypeDef* uart4Handle = &huart4;
static char messageBuf[MAX_PI_COMMS_SEND_LENGTH];

#define TX_BUFFER_SIZE 255		//size of the largest possible tx message
static uint8_t piComms_txBuffer[TX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions

#define RX_BUFFER_SIZE 128		//size of the largest possible rx message
static uint8_t piComms_rxBuffer[RX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions

static uint8_t rxByte = 0;		//pointer to where in the rxBuffer the next character will go
static uint8_t rxLen;
static bool gotLen;

static uint16_t rxPtrOffset;

static bool msgEndMode = false;	//true if we are in message end mode

static uint8_t delimiter = 0b0;
static uint8_t delimCtr;

KR23_OutgoingMessage TEST_PB_MESSAGE = KR23_OutgoingMessage_init_default;


// Circular Queue
#define QUEUE_MAX 256	//arbitrary number larger than the maximum number of transmissions we will receive before the task loop occurs
typedef struct PiCommsQueue_t
{
	uint32_t head;
	uint32_t tail;
	uint32_t count;
	KR23_OutgoingMessage data[QUEUE_MAX];
}PiCommsQueue_t;

PiCommsQueue_t piCommsQueue;	//queue of all received messages as PiCommsMessage_t


PRIVATE void PiComms_handleMessage(uint16_t protobufLen);
PRIVATE void PiCommsQueue_init(PiCommsQueue_t * q);
PRIVATE KR23_OutgoingMessage PiCommsQueue_dequeue(PiCommsQueue_t * q);
PRIVATE void PiCommsQueue_enqueue(PiCommsQueue_t * q, KR23_OutgoingMessage value);


PUBLIC void PiComms_Init(){
	__HAL_UART_FLUSH_DRREGISTER(uart4Handle);
	HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
	PiCommsQueue_init(&piCommsQueue);

	SerialDebug(TAG, "PiCommsModile Starting...");
}

//PUBLIC void PiComms_Send(const char * message, ...)
//{
//	va_list args;
//	va_start(args, message);
//	length_t len = vsprintf(messageBuf, message, args);
//	HAL_UART_Transmit(uart4Handle, (uint8_t*)messageBuf, len, HAL_MAX_DELAY);
//	va_end(args);
//
//}


PUBLIC void PiComms_Send(PiIncomingMessage_t im)
{
	uint16_t message_length;
	bool status;
	pb_ostream_t stream = pb_ostream_from_buffer(piComms_txBuffer, sizeof(piComms_txBuffer));

	status = pb_encode(&stream, KR23_IncomingMessage_fields, &im);
	message_length = stream.bytes_written;
	if (!status)
	{
		SerialDebug(TAG,"Failed to write to Buffer");
		return ;
	}
	piComms_txBuffer[message_length] = (uint8_t)delimiter;		//write termination characters
	piComms_txBuffer[message_length+1] = (uint8_t)delimiter;
	message_length += 2;

	//SerialDebug(TAG, "%s", piComms_txBuffer);

	//HAL_UART_Transmit(uart4Handle, piComms_txBuffer, message_length, HAL_MAX_DELAY);				//I remember someone (perhaps Mingy) saying HAL_MAX_DELAY may not be what we want here. I added updating this to my mcu to do list. - Eric E
}

void clearRx()
{
	gotLen = false;
	memset(piComms_rxBuffer, 0, RX_BUFFER_SIZE);
	rxPtrOffset = 0;
}

/*
 * message format is "<protobuf message>'term1''term2'"
 * We check for 'term1''term2' and consider anything that came before it to be a <protobuf message>
 * if by coincidence or error we receive 'term1''term2' and what comes before is not <protobuf message>, we discard the message, report the occurrence to topside to be logged there
 */
volatile uint8_t counter = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart != uart4Handle) return;

	__HAL_UART_FLUSH_DRREGISTER(uart4Handle);

	// message end mode: if we're not in sync with the delim'd frames, clear the current RX values and buffer and find the next delim'd frame

	if (msgEndMode)
	{
		uint8_t tmp = 1;
		while (delimCtr < 2)
		{
			HAL_UART_Receive(uart4Handle, (uint8_t*)&tmp, 1, 100);
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
		HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
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
		HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
		return;
	}

	// then check if we've received the length of the protobuf message
	if (!gotLen)
	{
		rxLen = rxByte;
		gotLen = true;
		HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
		return;
	}

	// then check if the length of the protobuf message is within bounds
	if (rxLen > RX_BUFFER_SIZE)
	{
		SerialDebug(TAG, "rxLen > RX_BUFFER_SIZE");
		clearRx();
		msgEndMode = true;
		HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
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
			HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
			return;
		}
		rxPtrOffset++;
		HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
		return;
	}

	// then check if the next two bytes are the delimiter, using a blocking receive, and if so, we've received the full message
	uint8_t tmp_1 = rxByte;
	    uint8_t tmp_2 = 3;
	    HAL_UART_Receive(uart4Handle, (uint8_t*)&tmp_2, 1, 100);
	    if (tmp_1 == delimiter && tmp_2 == delimiter)
	    {
	        // we've received the full message, so we can handle it
	        PiComms_handleMessage(rxLen);
	        clearRx();
	        HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
	        return;
	    }
	// if we've gotten here, we've received the full message but it's not delimited properly, so we drop the message and re-start
	SerialDebug(TAG, "message not delimited properly, dropping message, delims = %d%d", tmp_1, tmp_2);
	clearRx();
	msgEndMode = true;
	HAL_UART_Receive_IT(uart4Handle, &rxByte, 1);
	return;
	//SerialPrintln("HAL_UART_RxCpltCallback %p - %p = %d", piComms_rxBuffer_index, piComms_rxBuffer, (piComms_rxBuffer_index - piComms_rxBuffer));		//Extremely useful for debugging rx_Buffer
}

/*Allocates and assigns  */
PRIVATE void PiComms_handleMessage(uint16_t protobufLen)
{
	const pb_byte_t * pbBuffer = (pb_byte_t *)piComms_rxBuffer;
	KR23_OutgoingMessage recievedMessage;

	pb_istream_t istream = pb_istream_from_buffer(pbBuffer, protobufLen * sizeof(uint8_t));
	bool decodeSuccessful = pb_decode(&istream, KR23_OutgoingMessage_fields, &recievedMessage);
	if(decodeSuccessful){
		PiCommsQueue_enqueue(&piCommsQueue, recievedMessage);
	}else{
		SerialDebug(TAG, "Problem decoding Message");
	}
}



uint8_t PiComms_IsEmpty()
{
	if(piCommsQueue.count > 0) return 0;
	return 1;
}

KR23_OutgoingMessage PiComms_GetNext()
{
	return PiCommsQueue_dequeue(&piCommsQueue);
}

/*
 * message is sent in format #<id><length><data>!
 * # and ! are not saved since they are only used as deliminators
 * <id> is characters 0-3
 * <length> is the characters 4-7
 * <data> is the remainder
 */
void PiComms_ParseMessage(){

}

/*********************************************************************************
 *
 * 		Circular queue for serial input
 *
 **********************************************************************************/
PRIVATE void PiCommsQueue_init( PiCommsQueue_t * q )
{
	q->head = 0;
	q->count = 0;
	q->tail = 0;
}

PRIVATE void PiCommsQueue_enqueue( PiCommsQueue_t * q, KR23_OutgoingMessage msg )
{
	if(q->count < QUEUE_MAX)
	{
		// copy message struct into the queue
		q->data[q->tail] = msg;

		// increment count
		q->count ++;
		q->tail = (q->tail + 1) % QUEUE_MAX; // circular
	}
}

PRIVATE KR23_OutgoingMessage PiCommsQueue_dequeue( PiCommsQueue_t * q)
{
	if(q->count > 0)
	{
		KR23_OutgoingMessage returnValue = q->data[q->head];
		q->count --;
		q->head = (q->head + 1) % QUEUE_MAX;
		return returnValue;
	}
	KR23_OutgoingMessage failedMessage;
	failedMessage.has_attachmentCommand = false;
	failedMessage.has_thrusterCommand = false;
	return failedMessage;// garbage value, should never be returned
}
