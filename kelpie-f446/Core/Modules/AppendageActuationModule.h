/*
 * AppendageActuationModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef MODULES_APPENDAGEACTUATIONMODULE_H_
#define MODULES_APPENDAGEACTUATIONMODULE_H_

#include <HBridgeDriver.h>
#include "UserTypes.h"
#include "stm32f4xx_hal.h"

PUBLIC void AAMod_SetAppendageValue(claw_state_t state);
PUBLIC void AAMod_SetServoValue(ServoID_t id, uint8_t input);
PUBLIC void AAMod_SetLightValue(uint8_t input);

PUBLIC void AAMod_Callback(AttachmentCommand ac);

#endif /* MODULES_APPENDAGEACTUATIONMODULE_H_ */
