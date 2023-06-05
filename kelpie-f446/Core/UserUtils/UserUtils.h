/*
 * UserUtils.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Wring8
 */

#ifndef USERUTILS_USERUTILS_H_
#define USERUTILS_USERUTILS_H_

#include <UserTypes.h>

PUBLIC pwm_t MapInputToPWM(uint8_t input, const double scale, pwm_t minValue);
//PUBLIC uint16_t int8toHexInt16(uint8_t num);
//PUBLIC const char * floatToConstChar(float flt);


#endif /* USERUTILS_USERUTILS_H_ */
