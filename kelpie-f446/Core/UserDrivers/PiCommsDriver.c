/*
 * PiCommsDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingye
 */

#include "PiCommsDriver.h"

extern UART_HandleTypeDef huart4;
static char messageBuf[MAX_PI_COMMS_SEND_LENGTH];

static uint8_t piComms_rxBuffer[32]; // Max of 32 bytes of data

// Circular Queue
#define QUEUE_MAX 256
typedef struct PiCommsQueue_t
{
	uint32_t head;
	uint32_t tail;
	uint32_t count;
	uint8_t data[QUEUE_MAX];
}PiCommsQueue_t;

PiCommsQueue_t piCommsQueue;

PRIVATE void PiCommsQueue_init(PiCommsQueue_t * q);
PRIVATE uint8_t PiCommsQueue_dequeue(PiCommsQueue_t * q);
PRIVATE void PiCommsQueue_enqueue(PiCommsQueue_t * q, uint8_t value);

PUBLIC void PiComms_Init(){
	HAL_UART_Receive_IT(&huart4, piComms_rxBuffer, 1);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	PiCommsQueue_enqueue(&piCommsQueue, piComms_rxBuffer[0]);
    HAL_UART_Receive_IT(&huart4, piComms_rxBuffer, 1);

}

uint8_t PiComms_GetNextChar()
{
	return PiCommsQueue_dequeue(&piCommsQueue);
}

uint8_t PiComms_IsEmpty()
{
	if(piCommsQueue.count > 0) return 0;
	return 1;
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

PRIVATE void PiCommsQueue_enqueue( PiCommsQueue_t * q, uint8_t value )
{
	if(q->count < QUEUE_MAX)
	{
		// copy message struct into the queue
		q->data[q->tail]= value;

		// increment count
		q->count ++;
		q->tail = (q->tail + 1) % QUEUE_MAX; // circular
	}
}

PRIVATE uint8_t PiCommsQueue_dequeue( PiCommsQueue_t * q)
{
	if(q->count > 0)
	{
		uint8_t returnValue = q->data[q->head];
		q->count --;
		q->head = (q->head + 1) % QUEUE_MAX;
		return returnValue;
	}
	return 0xFF;// garbage value, should never be returned
}
