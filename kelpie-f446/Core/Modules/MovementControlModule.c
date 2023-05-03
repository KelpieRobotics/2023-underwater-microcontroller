/*
 * MovementControlModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "MovementControlModule.h"
#include "DataAggregationModule.h"
#include "PWMDriver.h"
#include "UserTypes.h"
#include "stdlib.h"
#include "PiCommsDriver.h"

#define TAG "MCM"

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

//takes uint8_t* and calls MCMod_SetThrusterValue with uint8_t id, uint8_t input
PUBLIC void MCMod_ThrusterCallback(ThrusterCommand tc)
{
	if(tc.has_thruster_0_PWM) MCMod_SetThrusterValue(0, tc.thruster_0_PWM);
	if(tc.has_thruster_1_PWM) MCMod_SetThrusterValue(1, tc.thruster_1_PWM);
	if(tc.has_thruster_2_PWM) MCMod_SetThrusterValue(2, tc.thruster_2_PWM);
	if(tc.has_thruster_3_PWM) MCMod_SetThrusterValue(3, tc.thruster_3_PWM);
	if(tc.has_thruster_4_PWM) MCMod_SetThrusterValue(4, tc.thruster_4_PWM);
	if(tc.has_thruster_5_PWM) MCMod_SetThrusterValue(5, tc.thruster_5_PWM);
	if(tc.has_thruster_6_PWM) MCMod_SetThrusterValue(6, tc.thruster_6_PWM);
	if(tc.has_thruster_7_PWM) MCMod_SetThrusterValue(7, tc.thruster_7_PWM);

	SerialDebug(TAG, "Set thrusters: %s%s%s%s%s%s%s%s",
			(tc.has_thruster_0_PWM ? "0, " : ""),
			(tc.has_thruster_1_PWM ? "1, " : ""),
			(tc.has_thruster_2_PWM ? "2, " : ""),
			(tc.has_thruster_3_PWM ? "3, " : ""),
			(tc.has_thruster_4_PWM ? "4, " : ""),
			(tc.has_thruster_5_PWM ? "5, " : ""),
			(tc.has_thruster_6_PWM ? "6, " : ""),
			(tc.has_thruster_7_PWM ? "7" : ""));

	PiComms_Send("#ACK:%s!", TAG);
}
