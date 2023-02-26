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
#include "stdlib.h"

//this works as long as the motor uses THRUSTER_SAFE_MIN_VALUE as full reverse, THRUSTER_SAFE_MAX_VALUE as full forward, and their average as stationary
const double PWM_SCALE = (double)(THRUSTER_SAFE_MAX_VALUE - THRUSTER_SAFE_MIN_VALUE) / (255.0);

/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void MCMod_SetThrusterValue(ThrusterID_t id, uint8_t input){
	//if the id values passed in are not the same as our local values, we will add a mapping to them.
	DA_SetThrusterValue(id, MapInputToPWM(input));
}

/*
 * Converts a uint8_t with 0 as full forward, 127 as inactive, and 255 as full reverse to a uint16_t with 1100 as full backward, 1500 as inactive, and 1900 as full forward
 */
PRIVATE pwm_t MapInputToPWM(uint8_t input){
	//SerialPrintln("\tMapInputToPWM: %d",THRUSTER_SAFE_MIN_VALUE + (pwm_t)(PWM_SCALE * input));
	return THRUSTER_SAFE_MIN_VALUE + (pwm_t)(PWM_SCALE * input);
}

//takes void* and calls MCMod_SetThrusterValue with uint8_t id, uint8_t input
PUBLIC void MCMod_ThrusterCallback(void *data)
{
	const char * p = (char*)data;						//converting from char-hex to one uint16_t then splitting the uint16_t in two.
	uint16_t idAndInput = strtol(p, NULL, 16);
	uint8_t * idAndInputArr = (uint8_t *)(&idAndInput);
	uint8_t id = idAndInputArr[1];
	uint8_t input = idAndInputArr[0];

	SerialPrintln("\tMCMod_ThrusterCallback id: %d, input: %d",id, input);
	MCMod_SetThrusterValue(id, input);
}
