/*
 * SafetyManagerTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "SafetyManagerModule.h"
#include "SafetyManagerTask.h"
#include "SerialDebugDriver.h"

// Function alias - replace with the driver api
#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

// FreeRTOS Configuration
#define STACK_SIZE 128*4
#define SAFETY_TASK_PRIORITY (osPriority_t) osPriorityHigh
#define TIMER_SAFETY_TASK 1000UL

// Forward Declaration
PUBLIC void InitSafetyTask(void);
PRIVATE void SafetyTask(void *argument);

osThreadId_t SafetyTaskHandle;
const osThreadAttr_t SafetyTask_attributes = {
	.name = "SafetyTask",
	.stack_size = STACK_SIZE,
	.priority = SAFETY_TASK_PRIORITY,
};

PUBLIC void InitSafetyTask(void)
{

	SafetyTaskHandle = osThreadNew(SafetyTask, NULL, &SafetyTask_attributes);

}
PRIVATE void SafetyTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	DebugPrint("safety");

	for(;;)
	{
		cycleTick += TIMER_SAFETY_TASK;
		osDelayUntil(cycleTick);
		DebugPrint("safety loop");

	}
}
