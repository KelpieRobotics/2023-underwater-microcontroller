/*
 * MovementControlTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "MovementControlModule.h"
#include "MovementControlTask.h"
#include "ThrusterDriver.h"
#include "SerialDebugDriver.h"

// Function alias - replace with the driver api
#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

// FreeRTOS Configuration
#define STACK_SIZE 128*8
#define MOVE_CTRL_TASK_PRIORITY (osPriority_t) osPriorityRealtime2
#define TIMER_MOVE_CTRL_TASK 500UL

osThreadId_t MovementControlTaskHandle;
const osThreadAttr_t MovementControlTask_attributes = {
	.name = "MovementControl",
	.stack_size = STACK_SIZE,
	.priority = MOVE_CTRL_TASK_PRIORITY,
};

// Forward declarations
PUBLIC void InitMovementControlTask(void);
PRIVATE void MovementControlTask(void *argument);

PUBLIC void InitMovementControlTask(void)
{
	MovementControlTaskHandle = osThreadNew(MovementControlTask, NULL, &MovementControlTask_attributes);
}

PRIVATE void MovementControlTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	DebugPrint("Movement Control Starting");
	for(;;)
	{
		cycleTick += TIMER_MOVE_CTRL_TASK;
		osDelayUntil(cycleTick);
		DebugPrint("Movement Control Loop");
		// debug testing here (temporary, driver should be moved to module)
		SetThrusterPWM(THRUSTER1, 1000);
		SetThrusterPWM(THRUSTER2, 1100);
		SetThrusterPWM(THRUSTER3, 1200);
		SetThrusterPWM(THRUSTER4, 1300);
		SetThrusterPWM(THRUSTER5, 1400);
		SetThrusterPWM(THRUSTER6, 1500);
		SetThrusterPWM(THRUSTER7, 1600);
		SetThrusterPWM(THRUSTER8, 1700);
	}
}
