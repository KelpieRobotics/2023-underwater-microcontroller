/*
 * NavigationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "NavigationModule.h"
#include "NavigationTask.h"
#include "SerialDebugDriver.h"

// Function alias - replace with the driver api
#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

// FreeRTOS Configuration
#define STACK_SIZE 128*8
#define NAV_TASK_PRIORITY (osPriority_t) osPriorityRealtime4
#define TIMER_NAV_TASK 500UL

osThreadId_t NavigationTaskHandle;
const osThreadAttr_t NavigationTask_attributes = {
	.name = "Navigation Task",
	.stack_size = STACK_SIZE,
	.priority = NAV_TASK_PRIORITY,
};

// Forward declarations
PUBLIC void InitNavigationTask(void)
{
	NavigationTaskHandle = osThreadNew(NavigationTask, NULL, &NavigationTask_attributes);
}
PRIVATE void NavigationTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	DebugPrint("Navigation Task Starting");
	for(;;)
	{
		cycleTick += TIMER_NAV_TASK;
		osDelayUntil(cycleTick);
		DebugPrint("Navigation Task Loop");
	}
}
