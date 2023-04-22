/*
 * NavigationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "NavigationModule.h"
#include "NavigationTask.h"
#include "MS5837Driver.h"
#include "SerialDebugDriver.h"
#include "DataAggregationModule.h"
#include "Adafruit_BNO080/IMUDriver.h"
#include "MS5837Driver.h"

#define TAG "NAT"

pressure_t pressure = 0;
depth_t depth = 0;

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



PRIVATE void NavigationTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	SerialDebug(TAG, "Navigation Task Starting...");
	result_t initIMURes = NAMod_IMUInit();
	result_t initPressureRes = NAMod_PressureInit();



	sh2_SensorValue_t values;
	for(;;)
	{
		cycleTick += TIMER_NAV_TASK;
		osDelayUntil(cycleTick);

		SerialDebug(TAG, "Navigation Task Loop");

		// if init was unsuccessful, try again
		if(initIMURes == RESULT_ERR)
		{
			SerialDebug(TAG, "Reattempting IMU init");
			initIMURes = NAMod_IMUInit();
		}
		else
		{
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

		if(initPressureRes != RESULT_OK)
		{
			initPressureRes = NAMod_PressureInit();
		}
		else
		{
			if (NAMod_PressureRoutine() != RESULT_OK)
			{
				SerialDebug(TAG, "Error getting PRESSURE SENSOR values");
			}
		}

	}
}
