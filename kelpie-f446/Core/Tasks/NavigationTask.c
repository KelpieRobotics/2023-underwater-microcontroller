/*
 * NavigationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "NavigationModule.h"
#include "NavigationTask.h"
#include "SerialDebugDriver.h"
#include "DataAggregationModule.h"
#include "Adafruit_BNO080/IMUDriver.h"

#define TAG "NAT"

// FreeRTOS Configuration
#define STACK_SIZE 1024*16
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


PRIVATE result_t NavigationIMUInit()
{
	SerialDebug(TAG, "IMU Init Sequence Starting");
	if(IMU_BeginI2C(0x4A,0)!= RESULT_OK)
	{
		SerialDebug(TAG, "Error with begin I2C");
		return RESULT_ERR;
	}
	else
	{
		SerialDebug(TAG, "I2C successful");
	}

	if(enableReport(SH2_ROTATION_VECTOR)!= RESULT_OK)
	{
		SerialDebug(TAG, "Error with setting quaternion reports");
		return RESULT_ERR;
	}
	else
	{
		SerialDebug(TAG, "Quaternion report set");
	}
	SerialDebug(TAG, "IMU init all successful");
	return RESULT_OK;
}

PRIVATE void NavigationTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	SerialDebug(TAG, "Navigation Task Starting...");
	result_t initRes = NavigationIMUInit();

	sh2_SensorValue_t values;
	for(;;)
	{
		cycleTick += TIMER_NAV_TASK;
		osDelayUntil(cycleTick);

		SerialDebug(TAG, "Navigation Task Loop");

		// if init was unsuccessful, try again
		if(initRes == RESULT_ERR)
		{
			SerialDebug(TAG, "Reattempting IMU init");
			initRes = NavigationIMUInit();
		}
		// if init still unsuccessful try again next cycle
		if(initRes == RESULT_ERR)
		{
			continue;
		}

		if (wasReset())
		{
		    SerialDebug(TAG, "Sensor was reset");
		    enableReport(SH2_ROTATION_VECTOR);
		}

		if(getSensorEvent(&values))
		{
			SerialDebug(TAG, "imu quat success");
			float i = values.un.rotationVector.i;
			float j = values.un.rotationVector.j;
			float k = values.un.rotationVector.k;
			float r = values.un.rotationVector.real;
			SerialDebug(TAG, "i:%.6f\tj:%.6f\tj:%.6f\tr:%.6f", i,j,k,r);

			DA_SetIMUQuaterion(i, j, k, r);
		}
		else
		{
			SerialDebug(TAG, "Error getting imu quaternion values");
		}

	}
}
