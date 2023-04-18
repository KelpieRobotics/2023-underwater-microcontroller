/*
 * MovementControlTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "MovementControlTask.h"
#include "PWMDriver.h"
#include "SerialDebugDriver.h"
#include "DataAggregationModule.h"

//TEMP
#include "AppendageActuationModule.h"

#define TAG "MCT"

extern TIM_HandleTypeDef htim3;

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
	uint8_t thrusterID = 0;
	uint32_t cycleTick = osKernelGetTickCount();

	SerialDebug(TAG, "Movement Control Starting...");
	ThrusterDriverInit();

	while(thrusterID < NUM_THRUSTERS){
		DA_SetThrusterValue(thrusterID++, GetThrusterInitValue());
	}
  
	/*
	result_t restest = SetThrusterPWM(LIGHTMODULE, 1100);
	vTaskDelay(1000/ portTICK_PERIOD_MS);
	SetThrusterPWM(LIGHTMODULE, 1500);*/

	SetLightModulePWM(1100);
	vTaskDelay(1000/ portTICK_PERIOD_MS);
	SetLightModulePWM(1900);

	SetServoPWM(SERVO1, 1500);
	SetServoPWM(SERVO2, 1500);
	SetServoPWM(SERVO3, 1500);
	vTaskDelay(5000/ portTICK_PERIOD_MS);

	for(int i= 0; i<3; i++)
	{
		SetServoPWM(SERVO1, 500);
		SetServoPWM(SERVO2, 500);
		SetServoPWM(SERVO3, 500);
		vTaskDelay(2000/ portTICK_PERIOD_MS);
		SetServoPWM(SERVO1, 2495);
		SetServoPWM(SERVO2, 2495);
		SetServoPWM(SERVO3, 2495);
		vTaskDelay(2000/ portTICK_PERIOD_MS);
	}
	for(;;)
	{
		cycleTick += TIMER_MOVE_CTRL_TASK;
		osDelayUntil(cycleTick);
		SerialDebug(TAG, "Movement Control Loop");

		// TEMP
		AAMod_SetAppendageValue(CLAW_OPEN);
		vTaskDelay(2000/ portTICK_PERIOD_MS);
		AAMod_SetAppendageValue(CLAW_CLOSE);
		vTaskDelay(2000/ portTICK_PERIOD_MS);
		AAMod_SetAppendageValue(CLAW_OPEN);

		//for each thruster, check DataAggregator info and update accordingly
		thrusterID = 0;
		while(thrusterID < NUM_THRUSTERS){
			SetThrusterPWM(thrusterID, DA_GetThrusterValue(thrusterID));
			thrusterID++;
		}
	}
}
