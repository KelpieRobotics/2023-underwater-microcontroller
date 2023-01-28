/*
 * MovementControlModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "MovementControlModule.h"
#include "DataAggregationModule.h"
#include "ThrusterDriver.h"
#include "UserTypes.h"

//this works as long as the motor uses THRUSTER_SAFE_MIN_VALUE as full reverse, THRUSTER_SAFE_MAX_VALUE as full forward, and their average as stationary
const PWM_SCALE = (THRUSTER_SAFE_MAX_VALUE - THRUSTER_SAFE_MIN_VALUE) / (255);

/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void MCMod_SetThrusterValue(ThrusterID_t id, uint8_t input){
	//if the id values passed in are not the same as our local values, we will add a mapping to them.
	DA_SetThrusterPower(id, MapInputToPWM(input));
}

/*
 * Converts a uint8_t with 0 as full back, 127 as inactive, and 255 as full forward to a uint16_t with 1100 as full backward, 1500 as inactive, and 1900 as full forward
 */
PRIVATE pwm_t MapInputToPWM(uint8_t input){
	return THRUSTER_SAFE_MIN_VALUE + PWM_SCALE * (pwm_t)input;
}
