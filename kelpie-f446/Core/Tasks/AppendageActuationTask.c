/*
 * AppendageActuationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include <HBridgeDriver.h>
#include "AppendageActuationTask.h"
#include "DataAggregationModule.h"
#include "SerialDebugDriver.h"

#define TAG "AAT"
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

/*
 * initializes the appendages, then checks the data aggregator for updates on the servo control
 */
PRIVATE void AppendageActuationTask(void *argument)
{
	uint8_t servoID;
	uint32_t cycleTick = osKernelGetTickCount();

	SerialDebug(TAG, "Actuator Control Starting...");
	ServoDriverInit();

	for(;;)
	{
		cycleTick += TIMER_APP_ACT_TASK;
		osDelayUntil(cycleTick);
		SerialDebug(TAG, "Actuator Control Loop");

		//for each thruster, check DataAggregator info and update accordingly
		servoID = 0;

		while(servoID < NUMBER_SERVOS){
			SetServoAngle(servoID, DA_GetAppendageState(servoID));
			servoID++;
		}

	}
}
