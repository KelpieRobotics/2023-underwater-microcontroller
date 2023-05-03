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

typedef enum {
	CLAW_IDLE,
	CLAW_CLOSE,
	CLAW_OPEN
} claw_state_t;

PUBLIC void AAMod_SetAppendageValue(claw_state_t state);
PUBLIC void AAMod_AppendageCallback(AttachmentCommand attachmentCommand);

#endif /* MODULES_APPENDAGEACTUATIONMODULE_H_ */
