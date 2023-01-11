/*
 * SafetyManagerTask.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef TASKS_SAFETYMANAGERTASK_H_
#define TASKS_SAFETYMANAGERTASK_H_

// STM headers
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

// Our headers
#include "UserTypes.h"

// STD headers
#include <stdint.h>

typedef enum{
	TEMPERATURE =0,
	HUMIDITY

} SafetyEvents_t;

typedef struct SafetyThresholds {
	SafetyEvents_t eventName;
	int32_t maxError;
	int32_t maxWarning;
	int32_t minError;
	int32_t minWarning;

} SafetyThresholds_t;

PUBLIC void InitSafetyTask(void);
PRIVATE void SafetyTask(void *argument);


#endif /* TASKS_SAFETYMANAGERTASK_H_ */
