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
static char term1 = 'K';	//somewhat arbitrary termination characters of message are chosen so that they are unlikely to occur sequentially in any messages
static char term2 = '.';	// this will be replaced with cobs bit stuffing so 0 is termination
bool lastWasTerm1 = false;

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

	memset(piComms_rxBuffer, NULL, RX_BUFFER_SIZE);		//clear buffer
	HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);		//get first length
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
	uint8_t message_length;
	bool status;
	pb_ostream_t stream = pb_ostream_from_buffer(&piComms_txBuffer[1], sizeof(piComms_txBuffer));		//first byte is length

	status = pb_encode(&stream, KR23_IncomingMessage_fields, &im);
	message_length = stream.bytes_written;
	if (!status)
	{
		SerialDebug(TAG,"Failed to write to Buffer");
		return ;
	}
	piComms_txBuffer[0] = message_length;						//write 0th byte as length
	piComms_txBuffer[message_length+1] = (uint8_t)term1;		//write termination characters
	piComms_txBuffer[message_length+2] = (uint8_t)term2;

	//SerialDebug(TAG, "%s", piComms_txBuffer);
	return;
	HAL_UART_Transmit(uart4Handle, piComms_txBuffer, message_length+3, HAL_MAX_DELAY);				//+3 for [0] length, [-2] term1, [-1] term2
}

/*
 * message format is "<protobuf message>'term1''term2'"
 * We check for 'term1''term2' and consider anything that came before it to be a <protobuf message>
 * if by coincidence or error we receive 'term1''term2' and what comes before is not <protobuf message>, we discard the message, report the occurrence to topside to be logged there
 */
int8_t len = -1;
typedef enum {
	GET_LENGTH,
	GET_DATA,
	CRAWL
}RxState;
RxState rxState = GET_LENGTH;
bool lastCrawlWas0 = false;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart != uart4Handle) return;

	/*crawl mode*/
	if(rxState == CRAWL){
		if(piComms_rxBuffer[0] == 0){
			if(lastCrawlWas0){
				//done
				rxState = GET_LENGTH;
				HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);
				return;
			}
			lastCrawlWas0 = true;
			HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);
			return;
		}
		HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);			//Detected a bug where get stuck in this state, but It may have been caused by a bad test environment.
		return;
	}

	/*len should be set*/
	if (rxState == GET_LENGTH){
        len = *piComms_rxBuffer;
        rxState = GET_DATA;
        HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, len + 2);	//protobuf length + 2 for termination 00
        return;
	}

	if(rxState == GET_DATA){
		if(piComms_rxBuffer[len] == 0 && piComms_rxBuffer[len+1] == 0){		//last 2 chars are 00 means successful message
			PiComms_handleMessage(len);
			memset(piComms_rxBuffer, NULL, RX_BUFFER_SIZE);		//clear buffer
			rxState = GET_LENGTH;
			HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);
			return;
		}

		memset(piComms_rxBuffer, NULL, RX_BUFFER_SIZE);		//clear buffer
		rxState = CRAWL;
		HAL_UART_Receive_IT(uart4Handle, piComms_rxBuffer, 1);
		return;
	}
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
