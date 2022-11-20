/*
 * AppendageActuationTask.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef TASKS_APPENDAGEACTUATIONTASK_H_
#define TASKS_APPENDAGEACTUATIONTASK_H_

// STM headers
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

// Our headers
#include "UserTypes.h"

// STD headers
#include <stdint.h>

PUBLIC void InitAppendageActuationTask(void);
PRIVATE void AppendageActuationTask(void *argument);

#endif /* TASKS_APPENDAGEACTUATIONTASK_H_ */
