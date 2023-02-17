/*
 * AppendageActuationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

// Our Libraries
#include "AppendageActuationModule.h"
#include "AppendageActuationTask.h"
#include "SerialDebugDriver.h"

// Function alias - replace with the driver api
#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

// FreeRTOS Configuration
#define STACK_SIZE 128*8
#define APP_ACT_TASK_PRIORITY (osPriority_t) osPriorityRealtime3
#define TIMER_APP_ACT_TASK 500UL

osThreadId_t AppendageActuationTaskHandle;
const osThreadAttr_t AppendageActuationTask_attributes = {
	.name = "AppendageActuation",
	.stack_size = STACK_SIZE,
	.priority = APP_ACT_TASK_PRIORITY,
};

// Forward declarations
PUBLIC void InitAppendageActuationTask(void);
PRIVATE void AppendageActuationTask(void *argument);

PUBLIC void InitAppendageActuationTask(void)
{
	AppendageActuationTaskHandle = osThreadNew(AppendageActuationTask, NULL, &AppendageActuationTask_attributes);
}
PRIVATE void AppendageActuationTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	DebugPrint("AppActStarting");
	for(;;)
	{
		cycleTick += TIMER_APP_ACT_TASK;
		osDelayUntil(cycleTick);
		DebugPrint("AppActLoop");
	}
}
