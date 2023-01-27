/*
 * MovementControlModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef MODULES_MOVEMENTCONTROLMODULE_H_
#define MODULES_MOVEMENTCONTROLMODULE_H_

#include "UserTypes.h"
#include "ThrusterDriver.h"

PUBLIC void MCMod_SetThrusterValue();
PRIVATE void MapInputToPWM();

#endif /* MODULES_MOVEMENTCONTROLMODULE_H_ */
