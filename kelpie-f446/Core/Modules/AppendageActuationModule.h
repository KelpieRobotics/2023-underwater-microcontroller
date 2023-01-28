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

PUBLIC void AAMod_SetAppendageValue(ServoID_t id, uint8_t input);
PRIVATE pwm_t AAMod_MapInputToPWM(uint8_t id, uint8_t input);

#endif /* MODULES_APPENDAGEACTUATIONMODULE_H_ */
