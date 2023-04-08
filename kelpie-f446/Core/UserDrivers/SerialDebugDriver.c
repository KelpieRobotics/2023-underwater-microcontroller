/*
 * SerialDebugDriver.c
 *
 *  Created on: Sep. 5, 2022
 *      Author: mingy
 */

#include "SerialDebugDriver.h"

extern UART_HandleTypeDef huart2;
static char messageBuf[MAX_SERIAL_PRINT_LENGTH];

static uint8_t serial_rxBuffer[32]; // Max of 32 bytes of data

// Circular Queue
#define QUEUE_MAX 32
typedef struct SerialQueue_t
{
	uint32_t head;
	uint32_t tail;
	uint32_t count;
	uint8_t data[QUEUE_MAX];
}SerialQueue_t;

SerialQueue_t serialQueue;

PRIVATE void SerialQueue_init(SerialQueue_t * q);
PRIVATE uint8_t SerialQueue_dequeue(SerialQueue_t * q);
PRIVATE void SerialQueue_enqueue(SerialQueue_t * q, uint8_t value);

PUBLIC void SerialDebugInit(){
	HAL_UART_Receive_IT(&huart2, serial_rxBuffer, 1);
	SerialQueue_init(&serialQueue);
}

PUBLIC void SerialPrint(const char * message, ...)
{
	va_list args;
	va_start(args, message);
	length_t len = vsprintf(messageBuf, message, args);
	HAL_UART_Transmit(&huart2, (uint8_t*)messageBuf, len, HAL_MAX_DELAY);
	va_end(args);

}

PUBLIC void SerialPrintln(const char * message, ...)
{
	va_list args;
	va_start(args, message);
	length_t len = vsprintf(messageBuf, message, args);
	messageBuf[len] = '\n';
	messageBuf[len+1] = '\r';
	HAL_UART_Transmit(&huart2, (uint8_t*)messageBuf, len+2, HAL_MAX_DELAY);
	va_end(args);
}
#define TAG_LEN 3
#define TAG_HEADER_LEN 6

PUBLIC void SerialDebug(const char * tag, const char * message, ...)
{
	va_list args;
	va_start(args, message);
	length_t len = vsprintf(messageBuf+TAG_HEADER_LEN, message, args);
	messageBuf[0] = '#';
	messageBuf[4] = ':';
	messageBuf[5] = ' ';
	for(int i = 0; i < TAG_LEN;i++)
	{
		messageBuf[i+1] = tag[i];
	}
	messageBuf[len+TAG_HEADER_LEN] = '\n';
	messageBuf[len+1+TAG_HEADER_LEN] = '\r';
	HAL_UART_Transmit(&huart2, (uint8_t*)messageBuf, len+TAG_HEADER_LEN+2, HAL_MAX_DELAY);
	va_end(args);
}

/* DO NOT USE, CONFLICTS WITH PI COMMS
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	SerialQueue_enqueue(&serialQueue, serial_rxBuffer[0]);
    HAL_UART_Receive_IT(&huart2, serial_rxBuffer, 1);

}
*/
uint8_t GetNextSerialDebugChar()
{
	return SerialQueue_dequeue(&serialQueue);
}

uint8_t IsSerialDebugEmpty()
{
	if(serialQueue.count > 0) return 0;
	return 1;
}

/*********************************************************************************
 *
 * 		Circular queue for serial input
 *
 **********************************************************************************/
PRIVATE void SerialQueue_init( SerialQueue_t * q )
{
	q->head = 0;
	q->count = 0;
	q->tail = 0;
}

PRIVATE void SerialQueue_enqueue( SerialQueue_t * q, uint8_t value )
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

PRIVATE uint8_t SerialQueue_dequeue( SerialQueue_t * q)
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
