/*
 * PiCommsDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingye eric
 */

#include "PiCommsDriver.h"
#include "SerialDebugDriver.h"
#include "UserTypes.h"

#include "DataAggregationModule.h"
#include "stdlib.h"

#define TAG "PCM"

static uint8_t uartRxByte;

extern UART_HandleTypeDef huart4;
UART_HandleTypeDef* uart4Handle = &huart4;

#define TX_BUFFER_SIZE 255		//size of the largest possible tx message
static uint8_t piComms_txBuffer[TX_BUFFER_SIZE]; 			//pointer to buffer that holds incoming transmissions


KR23_OutgoingMessage TEST_PB_MESSAGE = KR23_OutgoingMessage_init_default;

#define QUEUE_MAX 256	//arbitrary number larger than the maximum number of transmissions we will receive before the task loop occurs
typedef struct PiCommsQueue_t
{
	uint32_t head;
	uint32_t tail;
	uint32_t count;
	uint8_t data[QUEUE_MAX];
}PiCommsQueue_t;

PiCommsQueue_t piCommsQueue;	//queue of all received messages as PiCommsMessage_t
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

PRIVATE uint32_t PiCommsQueue_getCount(PiCommsQueue_t * q)
{
    return q->count;
}

PRIVATE void PiCommsQueue_enqueue( PiCommsQueue_t * q, uint8_t msg )
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

PRIVATE uint8_t PiCommsQueue_dequeue( PiCommsQueue_t * q)
{
	if(q->count > 0)
	{
		uint8_t returnValue = q->data[q->head];
		q->count --;
		q->head = (q->head + 1) % QUEUE_MAX;
		return returnValue;
	}
	return 0;// garbage value, should never be returned
}
PUBLIC void PiComms_ResetByteQueue()
{
	PiCommsQueue_init(&piCommsQueue);
}
PUBLIC uint8_t PiComms_DequeueByte()
{
	return PiCommsQueue_dequeue(&piCommsQueue);
}

PUBLIC uint32_t PiComms_GetByteQueueCount()
{
	return piCommsQueue.count;
}

PUBLIC void PiComms_Init(){
	__HAL_UART_FLUSH_DRREGISTER(uart4Handle);
	HAL_UART_Receive_IT(uart4Handle, &uartRxByte, 1);

	SerialDebug(TAG, "PiCommsModile Starting...");
}

PUBLIC void PiComms_Send(PiIncomingMessage_t im)
{
	/*
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

	//HAL_UART_Transmit(uart4Handle, piComms_txBuffer, message_length, HAL_MAX_DELAY);
	 *
	 */			//I remember someone (perhaps Mingy) saying HAL_MAX_DELAY may not be what we want here. I added updating this to my mcu to do list. - Eric E
}

/*
 * message format is "<protobuf message>'term1''term2'"
 * We check for 'term1''term2' and consider anything that came before it to be a <protobuf message>
 * if by coincidence or error we receive 'term1''term2' and what comes before is not <protobuf message>, we discard the message, report the occurrence to topside to be logged there
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart != uart4Handle) return;

	__HAL_UART_FLUSH_DRREGISTER(uart4Handle);

    //HAL_UART_Receive(uart4Handle, &rxData, 1, 100);

	PiCommsQueue_enqueue(&piCommsQueue, uartRxByte);
	//HAL_UART_AbortReceive(uart4Handle);
	HAL_UART_Receive_IT(uart4Handle, &uartRxByte, 1);
	return;
	//SerialPrintln("HAL_UART_RxCpltCallback %p - %p = %d", piComms_rxBuffer_index, piComms_rxBuffer, (piComms_rxBuffer_index - piComms_rxBuffer));		//Extremely useful for debugging rx_Buffer
}

void PiCommsUARTAbort()
{
	HAL_UART_AbortReceive(uart4Handle);
	HAL_UART_Receive_IT(uart4Handle, &uartRxByte, 1);
}

