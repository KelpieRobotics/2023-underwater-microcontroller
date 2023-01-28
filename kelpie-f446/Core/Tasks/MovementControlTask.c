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

/*
 * initializes the motors, then checks the data aggregator for updates on the motor control
 */
PRIVATE void MovementControlTask(void *argument)
{
	uint8_t thrusterID;
	uint32_t cycleTick = osKernelGetTickCount();

	DebugPrint("Movement Control Starting");
	ThrusterDriverInit();

	for(;;)
	{
		cycleTick += TIMER_MOVE_CTRL_TASK;
		osDelayUntil(cycleTick);
		DebugPrint("Movement Control Loop");

		//for each thruster, check DataAggregator info and update accordingly
		thrusterID = 0;
		while(thrusterID < NUM_THRUSTERS){
			SetThrusterPWM(thrusterID, DA_GetThrusterValue(thrusterID));
			thrusterID++;
		}
	}
}
