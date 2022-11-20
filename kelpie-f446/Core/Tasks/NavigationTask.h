/*
 * NavigationTask.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef TASKS_NAVIGATIONTASK_H_
#define TASKS_NAVIGATIONTASK_H_

// STM headers
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

// Our headers
#include "UserTypes.h"

// STD headers
#include <stdint.h>

PUBLIC void InitNavigationTask(void);
PRIVATE void NavigationTask(void *argument);

#endif /* TASKS_NAVIGATIONTASK_H_ */
