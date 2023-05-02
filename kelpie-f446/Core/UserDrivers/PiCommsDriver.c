/*
 * PiCommsDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingye eric
 */

#include "PiCommsDriver.h"
#include "SerialDebugDriver.h"
#include "UserTypes.h"
#include "..\..\..\Middlewares\Third_Party\NanoPB\DataTypes.pb.h"
#include "..\..\..\Middlewares\Third_Party\NanoPB\pb_decode.h"

#include "stdlib.h"

#define TAG "PCM"

extern UART_HandleTypeDef huart4;
static char messageBuf[MAX_PI_COMMS_SEND_LENGTH];

#define RX_BUFFER_SIZE 128		//number larger than the maximum number characters in the largest transmission we will receive + MESSAGE_ID_SIZE + MESSAGE_LENGTH_SIZE
static uint8_t piComms_rxBuffer[RX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions
static uint8_t *piComms_rxBuffer_index;		//pointer to where in the rxBuffer the next character will go
bool lastWasReturn = false;
bool decodeSuccessful = false;

KelpieRobotics_2023_OutgoingMessage TEST_PB_MESSAGE = KelpieRobotics_2023_OutgoingMessage_init_default;


// Circular Queue
#define QUEUE_MAX 256	//arbitrary number larger than the maximum number of transmissions we will receive before the task loop occurs
typedef struct PiCommsQueue_t
{
	uint32_t head;
	uint32_t tail;
	uint32_t count;
	PiCommsMessage_t data[QUEUE_MAX];
}PiCommsQueue_t;

PiCommsQueue_t piCommsQueue;	//queue of all received messages as PiCommsMessage_t


PRIVATE KelpieRobotics_2023_OutgoingMessage PiComms_rxBufferToMessage(uint16_t protobufLen);
PRIVATE void PiCommsQueue_init(PiCommsQueue_t * q);
PRIVATE PiCommsMessage_t PiCommsQueue_dequeue(PiCommsQueue_t * q);
PRIVATE void PiCommsQueue_enqueue(PiCommsQueue_t * q, PiCommsMessage_t value);

PUBLIC void PiComms_Init(){
	piComms_rxBuffer_index = piComms_rxBuffer;

	HAL_UART_Receive_IT(&huart4, piComms_rxBuffer_index, 1);
	PiCommsQueue_init(&piCommsQueue);

	TEST_PB_MESSAGE.id = 10;
	SerialDebug(TAG, "PiCommsModile Starting...");
}

PUBLIC void PiComms_Send(const char * message, ...)
{
	va_list args;
	va_start(args, message);
	length_t len = vsprintf(messageBuf, message, args);
	HAL_UART_Transmit(&huart4, (uint8_t*)messageBuf, len, HAL_MAX_DELAY);
	va_end(args);

}

/*
 * message format is "<protobuf message>\r\n"
 * We check for "\r\n" and consider anything that came before it to be a <protobuf message>
 * if by coincidence or error we receive "\r\n" and what comes before is not <protobuf message>, we discard the message, report the occurrence to topside to be logged there
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	SerialDebug(TAG, "HAL_UART_RxCpltCallback %d", piComms_rxBuffer_index[0]);
	uint8_t recievedByte = piComms_rxBuffer_index[0];
	if(lastWasReturn && (recievedByte == '\n')){
			SerialPrintln("#DEBUG: lastMessage: %d",TEST_PB_MESSAGE.id);		//Extremely useful for debugging rx_Buffer
		uint16_t protobufLen = piComms_rxBuffer_index - piComms_rxBuffer - 1;		//protobufLen = length the data received + 1 - 2. The 1 comes from an inclusive difference between the base and incremented pointer. The -2 comes from removing the last 2 characters "\r\n"
			SerialPrintln("#DEBUG: HAL_UART_RxCpltCallback message: %s",piComms_rxBuffer);		//Extremely useful for debugging rx_Buffer
			SerialPrintln("#DEBUG: protobufLen: %d",protobufLen);		//Extremely useful for debugging rx_Buffer
		piComms_rxBuffer_index = piComms_rxBuffer;					//reset piComms_rxBuffer_index if it is not the last byte in a message
		PiComms_rxBufferToMessage(protobufLen);						//make message
		memset(piComms_rxBuffer, '\0', (protobufLen+2) * sizeof(uint8_t));		//reset buffer
	} else {
		piComms_rxBuffer_index++;		//increment piComms_rxBuffer_index if it is not the last byte in a message
	}

	lastWasReturn = (recievedByte == '\r');		//set lastWasReturn

	HAL_UART_Receive_IT(&huart4, piComms_rxBuffer_index, 1);	//ready to receive next byte
	//SerialPrintln("#DEBUG: HAL_UART_RxCpltCallback message: %s",piComms_rxBuffer);		//Extremely useful for debugging rx_Buffer
}

/*Allocates and assigns  */
PRIVATE KelpieRobotics_2023_OutgoingMessage PiComms_rxBufferToMessage(uint16_t protobufLen)
{
	const pb_byte_t * pbBuffer = (pb_byte_t *)piComms_rxBuffer;
	KelpieRobotics_2023_OutgoingMessage recievedMessage;

	pb_istream_t istream = pb_istream_from_buffer(pbBuffer, protobufLen);
	decodeSuccessful = pb_decode(&istream, KelpieRobotics_2023_OutgoingMessage_fields, &recievedMessage);
	return recievedMessage;
}



uint8_t PiComms_IsEmpty()
{
	if(piCommsQueue.count > 0) return 0;
	return 1;
}

PiCommsMessage_t PiComms_GetNext()
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

PRIVATE void PiCommsQueue_enqueue( PiCommsQueue_t * q, PiCommsMessage_t msg )
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

PRIVATE PiCommsMessage_t PiCommsQueue_dequeue( PiCommsQueue_t * q)
{
	if(q->count > 0)
	{
		PiCommsMessage_t returnValue = q->data[q->head];
		q->count --;
		q->head = (q->head + 1) % QUEUE_MAX;
		return returnValue;
	}
	PiCommsMessage_t failedMessage;
	failedMessage.messageId = -1;
	failedMessage.dataLen = -1;
	return failedMessage;// garbage value, should never be returned
}
