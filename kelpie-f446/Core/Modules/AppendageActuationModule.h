/*
 * AppendageActuationModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef MODULES_APPENDAGEACTUATIONMODULE_H_
#define MODULES_APPENDAGEACTUATIONMODULE_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"
#include "ServoDriver.h"

typedef enum {
	CLAW_OPEN = -1,
	CLAW_IDLE,
	CLAW_CLOSE
} claw_state_t;

PUBLIC void AAMod_SetAppendageValue(claw_state_t state);

#endif /* MODULES_APPENDAGEACTUATIONMODULE_H_ */
