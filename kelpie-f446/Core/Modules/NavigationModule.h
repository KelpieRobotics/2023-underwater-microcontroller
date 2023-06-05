/*
 * NavigationModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef MODULES_NAVIGATIONMODULE_H_
#define MODULES_NAVIGATIONMODULE_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"

PUBLIC result_t NAMod_PressureInit();
PUBLIC result_t NAMod_PressureRoutine();
PRIVATE result_t NAMod_IMUInit();

#endif /* MODULES_NAVIGATIONMODULE_H_ */
