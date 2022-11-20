/*
 * InterCommsTask.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef TASKS_INTERCOMMSTASK_H_
#define TASKS_INTERCOMMSTASK_H_

// STM headers
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

// Our headers
#include "UserTypes.h"

// STD headers
#include <stdint.h>

PUBLIC void InitInternalCommsTask(void);
PRIVATE void InternalCommsTask(void *argument);


#endif /* TASKS_INTERCOMMSTASK_H_ */
