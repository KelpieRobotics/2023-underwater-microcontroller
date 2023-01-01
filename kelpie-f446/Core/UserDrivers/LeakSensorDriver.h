/*
 * LeakSensorDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERDRIVERS_LEAKSENSORDRIVER_H_
#define USERDRIVERS_LEAKSENSORDRIVER_H_

#include "UserTypes.h"

PUBLIC result_t InitLeakSensor();
PRIVATE result_t LeakSensorIntCallback();

#endif /* USERDRIVERS_LEAKSENSORDRIVER_H_ */
