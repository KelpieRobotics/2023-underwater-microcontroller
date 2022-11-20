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

PUBLIC void InitSafetyTask(void);
PRIVATE void SafetyTask(void *argument);


#endif /* TASKS_SAFETYMANAGERTASK_H_ */
