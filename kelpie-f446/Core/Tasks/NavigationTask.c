/*
 * NavigationTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "NavigationModule.h"
#include "NavigationTask.h"
#include "SerialDebugDriver.h"
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
PRIVATE void NavigationTask(void *argument)
{
	uint32_t cycleTick = osKernelGetTickCount();
	SerialDebug(TAG, "Navigation Task Starting...");
	if(IMU_BeginI2C(0x4A,0)!= RESULT_OK)
	{
		SerialDebug(TAG, "Error with begin i2c");
	}
	else
	{
		SerialDebug(TAG, "GOOD 1");
	}
	if(enableReport(SH2_ROTATION_VECTOR)!= RESULT_OK)
	{
		SerialDebug(TAG, "Error with set reports");
	}
	else
	{
		SerialDebug(TAG, "GOOD 2");
	}

	sh2_SensorValue_t values;
	for(;;)
	{
		cycleTick += TIMER_NAV_TASK;
		osDelayUntil(cycleTick);
		if (wasReset()) {
		    SerialDebug(TAG, "sensor was reset ");
		    enableReport(SH2_ROTATION_VECTOR);
		  }
		SerialDebug(TAG, "Navigation Task Loop");
		if(getSensorEvent(&values))
		{
			SerialDebug(TAG, "SUCCESS!");
			int32_t i = values.un.rotationVector.i*10000;
			int32_t j = values.un.rotationVector.j*10000;
			int32_t k = values.un.rotationVector.k*10000;
			int32_t r = values.un.rotationVector.real*10000;
			SerialDebug(TAG, "i:%d\tj:%d\tj:%d\tr:%d", i,j,k,r);
		}
		else
		{
			SerialDebug(TAG, "BAD!");
		}

	}
}
