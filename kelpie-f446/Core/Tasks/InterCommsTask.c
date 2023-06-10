/*
 * InterCommsTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */
#include "InterCommsTask.h"
#include "InterCommsModule.h"
#include "SerialDebugDriver.h"
#include "UserTypes.h"

// temp
#include "DataAggregationModule.h"
#include "MovementControlModule.h"
#include "AppendageActuationModule.h"

#include <stdio.h>
#include <stdlib.h>

#define TAG "ICT"


#define STACK_SIZE 1024*8
#define INTERNAL_COMMS_TASK_PRIORITY (osPriority_t) osPriorityRealtime1
#define TIMER_INTERNAL_COMMS_TASK 10UL

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

// Anything that needs to be sent out regularly should be called here.
// this function gets called once a cycle
PRIVATE void ICommsTransmitRoutine()
{
	IMUData_t imudata = DA_GetIMUQuaterion();
	//PiComms_Send("#QUI:%.6f#QUJ:%.6f#QUK:%.6f#QUR:%.6f\n\r", imudata.quat_i, imudata.quat_j, imudata.quat_k, imudata.quat_real);

	//humidity_t humidity = DA_GetHumidity();
	//temperature_t temperature = DA_GetTemperature();
	//PiComms_Send("#HUM:%.6f#TEM:%.6f\n\r", humidity, temperature);

	KR23_IncomingMessage imSensors = KR23_IncomingMessage_init_zero;
	imSensors.has_sensorsData = true;

//	imSensors.sensorsData.accelerometer_x = 0;
//	imSensors.sensorsData.accelerometer_y = 0;								//Still need these
//	imSensors.sensorsData.accelerometer_y = 0;

	imSensors.sensorsData.temperature = (double)DA_GetTemperature();
	imSensors.sensorsData.humidity = (double)DA_GetHumidity();
//	imSensors.sensorsData.pressure = 0;
//	imSensors.sensorsData.depth = 0;

	imSensors.sensorsData.quaternion_real = imudata.quat_real;
	imSensors.sensorsData.quaternion_i = imudata.quat_i;
	imSensors.sensorsData.quaternion_j = imudata.quat_j;
	imSensors.sensorsData.quaternion_k = imudata.quat_k;

	imSensors.sensorsData.result = KR23_KelpieResult_OK;

	IComms_Send(imSensors);
}

PRIVATE void InternalCommsTask(void *argument)
{

	uint32_t cycleTick = osKernelGetTickCount();
	SerialDebug(TAG, "InterCommsTask Starting...");
	InitInternalCommsModule();


	for(;;)
	{
		cycleTick += TIMER_INTERNAL_COMMS_TASK;
		osDelayUntil(cycleTick);
		//SerialDebug(TAG, "InterCommsTask Loop");

		// check if anything to process
		IComms_PollRxMessage();

		ICommsTransmitRoutine();
	}
}
