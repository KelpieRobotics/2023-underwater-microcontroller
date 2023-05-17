/*
 * SafetyManagerTask.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "SafetyManagerModule.h"
#include "SafetyManagerTask.h"
#include "SerialDebugDriver.h"
#include "LeakSensorDriver.h"
#include "HBridgeDriver.h"
#include "PWMDriver.h"
#include "stm32f4xx_hal.h"
#define TAG "SMT"

// FreeRTOS Configuration
#define STACK_SIZE 1024*2
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
	SerialDebug(TAG, "Safety Manager Task Starting...");

	safetySensorsInit();
	uint32_t commitSelfResetTimer = 0;

	for(;;)
	{

		cycleTick += TIMER_SAFETY_TASK;
		osDelayUntil(cycleTick);
		commitSelfResetTimer++;
		if(commitSelfResetTimer == 120)
		{
			SerialPrintln("========AAAAAAAAAaaaaaaaaa========");
			PWMDriverThrustersInit();
			vTaskDelay(50);
			HAL_NVIC_SystemReset();

		}
		SerialDebug(TAG, "Safety loop");

		updateSafetySensorRoutine();
	}
}
