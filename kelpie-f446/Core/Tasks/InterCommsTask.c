/*
 * InterCommsTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */
#include "InterCommsTask.h"
#include "InterCommsModule.h"
#include "SerialDebugDriver.h"
#include "PiCommsDriver.h"

#include <stdio.h>
#include <stdlib.h>

// Function alias - replace with the driver api
#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

#define STACK_SIZE 128*8
#define INTERNAL_COMMS_TASK_PRIORITY (osPriority_t) osPriorityRealtime1
#define TIMER_INTERNAL_COMMS_TASK 100UL

PUBLIC void InitInternalCommsTask(void);
PRIVATE void InternalCommsTask(void *argument);

osThreadId_t InternalCommsTaskHandle;
const osThreadAttr_t InternalCommsTask_attributes = {
	.name = "InternalCommunications",
	.stack_size = STACK_SIZE,
	.priority = INTERNAL_COMMS_TASK_PRIORITY,
};

PUBLIC void InitInternalCommsTask(void)
{

	InternalCommsTaskHandle = osThreadNew(InternalCommsTask, NULL, &InternalCommsTask_attributes);

}
PRIVATE void InternalCommsTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	DebugPrint("icomms");
	InitInternalCommsModule();

	uint8_t *data = malloc(sizeof(uint8_t));
	*data = 8;

	for(;;)
	{
		cycleTick += TIMER_INTERNAL_COMMS_TASK;
		osDelayUntil(cycleTick);
		//DebugPrint("icomms loop");

		//SerialPrintln("InternalCommsTask: %d", *data);
		//InternalCommsMessageCallback(4, 8, data);

		PiComms_Send("\tMessage sent");

		// pi comms demo code, temporary
		char temp[32] = {'-'};
		uint8_t index = 0;
		if(!PiComms_IsEmpty()){
			while(!PiComms_IsEmpty())
			{
				temp[index] = (char)PiComms_GetNextChar();
				PiComms_Send("Temp: %c \n", temp[index]);
				index++;
			}

		}


	}
}
