/*
 * MovementControlTask.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef TASKS_MOVEMENTCONTROLTASK_H_
#define TASKS_MOVEMENTCONTROLTASK_H_

// STM headers
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

// Our headers
#include "UserTypes.h"

// STD headers
#include <stdint.h>

PUBLIC void InitMovementControlTask(void);
PRIVATE void MovementControlTask(void *argument);

#endif /* TASKS_MOVEMENTCONTROLTASK_H_ */
