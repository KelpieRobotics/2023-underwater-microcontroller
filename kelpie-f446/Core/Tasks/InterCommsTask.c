/*
 * InterCommsTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */
#include "InterCommsTask.h"
#include "InterCommsModule.h"
#include "SerialDebugDriver.h"
#include "PiCommsDriver.h"
#include "UserTypes.h"
#include "DataAggregationModule.h"

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
	PiComms_Send("#QUI:%.6f#QUJ:%.6f#QUK:%.6f#QUR:%.6f\n\r", imudata.quat_i, imudata.quat_j, imudata.quat_k, imudata.quat_real);

	humidity_t humidity = DA_GetHumidity();
	temperature_t temperature = DA_GetTemperature();
	PiComms_Send("#HUM:%.6f#TEM:%.6f\n\r", humidity, temperature);



}
PRIVATE void InternalCommsTask(void *argument)
{
	PiCommsMessage_t msg;
	uint32_t cycleTick = osKernelGetTickCount();
	SerialDebug(TAG, "InterCommsTask Starting...");
	InitInternalCommsModule();

	uint8_t *data = malloc(sizeof(uint8_t));
	*data = 8;

	for(;;)
	{
		cycleTick += TIMER_INTERNAL_COMMS_TASK;
		osDelayUntil(cycleTick);
		//SerialDebug(TAG, "InterCommsTask Loop");

		while(!PiComms_IsEmpty())			//for each message, call it's callback method
		{
			msg = PiComms_GetNext();
			if(InternalCommsMessageCallback(msg) == RESULT_ERR)
			{
				SerialDebug(TAG,"ERR: InternalCommsTask message callback failed");
			}
			free(msg.data);						//free msg.data that is allocated in driver
		}
		ICommsTransmitRoutine();
	}
}
