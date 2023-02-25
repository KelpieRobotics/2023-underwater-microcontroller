/*
 * PiCommsDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingye eric
 */

#include "PiCommsDriver.h"
#include "SerialDebugDriver.h"
#include "UserTypes.h"

#include "stdlib.h"

extern UART_HandleTypeDef huart4;
static char messageBuf[MAX_PI_COMMS_SEND_LENGTH];

#define MESSAGE_ID_SIZE 4		//number of bytes of message ID
#define MESSAGE_LENGTH_SIZE 4	//number of bytes of message length
#define MESSAGE_ID_BASE 16		//base of message ID and length are hex

#define RX_BUFFER_SIZE 128		//number larger than the maximum number characters in the largest transmission we will receive + MESSAGE_ID_SIZE + MESSAGE_LENGTH_SIZE
static uint8_t piComms_rxBuffer[RX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions
static uint8_t *piComms_rxBuffer_index;		//pointer to where in the rxBuffer the next character will go


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


PRIVATE PiCommsMessage_t PiComms_rxBufferToMessage();
PRIVATE void PiCommsQueue_init(PiCommsQueue_t * q);
PRIVATE PiCommsMessage_t PiCommsQueue_dequeue(PiCommsQueue_t * q);
PRIVATE void PiCommsQueue_enqueue(PiCommsQueue_t * q, PiCommsMessage_t value);

PUBLIC void PiComms_Init(){
	piComms_rxBuffer_index = piComms_rxBuffer;

	HAL_UART_Receive_IT(&huart4, piComms_rxBuffer_index, 1);
	PiCommsQueue_init(&piCommsQueue);
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
 * if # clear buffer
 * if ! enqueue to queue
 * else add to buffer
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(piComms_rxBuffer_index - piComms_rxBuffer > RX_BUFFER_SIZE){
		SerialPrintln("#ERR: HAL_UART_RxCpltCallback BUFFER FULL");
		if(piComms_rxBuffer_index[0] != '!'){
			HAL_UART_Receive_IT(&huart4, piComms_rxBuffer_index, 1);
			return;
		}
	}

	switch (piComms_rxBuffer_index[0]){
		case '#':
			piComms_rxBuffer_index[0] = '\0';	//revert last char so it isn't sent forward
			break;
		case '!':
			piComms_rxBuffer_index[0] = '\0';	//revert last char so it isn't sent forward

			uint8_t rxLen = piComms_rxBuffer_index - piComms_rxBuffer;
			if(rxLen < MESSAGE_ID_SIZE + MESSAGE_LENGTH_SIZE){		//handle incomplete transmissions
				SerialPrintln("#ERR: HAL_UART_RxCpltCallback rx buffer message missing information, message: %s, len: %d",piComms_rxBuffer, rxLen);
			} else{
				PiCommsQueue_enqueue(&piCommsQueue, PiComms_rxBufferToMessage());	//convert rx buffer to message and enqueue in message queue
			}
			piComms_rxBuffer_index = piComms_rxBuffer;							//reset piComms_rxBuffer
			memset(piComms_rxBuffer, '\0', rxLen * sizeof(uint8_t));	//clear rx buffer
			break;
//		case '\r':		//THIS IS NEEDED FOR DEBUGGING IF \r CHARACTER IS SENT AS PART OF MESSAGE
//			break;
		default:
			piComms_rxBuffer_index++;
			break;
	}

	HAL_UART_Receive_IT(&huart4, piComms_rxBuffer_index, 1);
}

/*Allocates and assigns  */
PRIVATE PiCommsMessage_t PiComms_rxBufferToMessage()
{
	char * p = (char *)piComms_rxBuffer;		//Make idStr to hold id characters
	char idBuilder[MESSAGE_ID_SIZE];
	for(int i = 0; i<MESSAGE_ID_SIZE; i++){
		idBuilder[i] = p[i];
	}
	const char * idStr = idBuilder;
	p+=MESSAGE_ID_SIZE*sizeof(char);
	char * endIDPtr = p;

	char lenBuilder[MESSAGE_LENGTH_SIZE];		//Make lenStr to hold length characters
	for(int i = 0; i<MESSAGE_LENGTH_SIZE; i++){
		lenBuilder[i] = p[i];
	}
	const char * lenStr = lenBuilder;
	p+=MESSAGE_LENGTH_SIZE*sizeof(char);
	char * endLengthPtr = p;
	const char * messageStr = p;				//Make messageStr to hold message data

	PiCommsMessage_t msg = {									//make PiComms Message
	.messageId = strtol(idStr, &endIDPtr, MESSAGE_ID_BASE),
	.dataLen = strtol(lenStr, &endLengthPtr, MESSAGE_ID_BASE),
	.data = NULL};

	msg.data = malloc(msg.dataLen*sizeof(char));				//allocate memory for msg.data

	memcpy(msg.data, messageStr, msg.dataLen*sizeof(char));		//copy messageStr to msg.data

	return msg;
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
