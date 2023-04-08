/*
 * MovementControlModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef MODULES_MOVEMENTCONTROLMODULE_H_
#define MODULES_MOVEMENTCONTROLMODULE_H_

#include "UserTypes.h"
#include "PWMDriver.h"

PUBLIC void MCMod_SetThrusterValue(ThrusterID_t id, uint8_t input);
PRIVATE pwm_t MapInputToPWM(uint8_t input);
PUBLIC void MCMod_ThrusterCallback(uint8_t *data);

#endif /* MODULES_MOVEMENTCONTROLMODULE_H_ */
