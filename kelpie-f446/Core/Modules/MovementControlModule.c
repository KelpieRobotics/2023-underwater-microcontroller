/*
 * MovementControlModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "DataAggregationModule.h"
#include "MovementControlModule.h"
#include "ThrusterDriver.h"

const PWM_SCALE = (THRUSTER_SAFE_MAX_VALUE - THRUSTER_SAFE_MIN_VALUE) / (255);

//set DB values
//Module: SetPWMValue(ThrusterID, val)
//val is uint8_t 0-255 and maps to uint16_t 1100-1900

/*
 * controls the thruster motors
 */
PUBLIC void SetPWMValue(ThrusterID_t id){
	SetThrusterPWM(id, MapInputToPWM(GetThrusterPower(id)));
}

/*
 * Converts a uint8_t with 0 as full back, 127 as inactive, and 255 as full forward to a uint16_t with 1100 as full backward, 1500 as inactive, and 1900 as full forward
 */
PRIVATE uint16_t MapInputToPWM(uint8_t input){
	return THRUSTER_SAFE_MIN_VALUE + PWM_SCALE * (uint16_t)input;
}
