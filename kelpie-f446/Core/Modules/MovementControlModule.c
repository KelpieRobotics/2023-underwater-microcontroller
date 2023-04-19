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
#include <UserUtils.h>
#include "stdlib.h"
#include "PiCommsDriver.h"

#define TAG "MCM"

/*
 * converts the input to thruster pwm value, then calls DA_SetThrusterValue
 */
PUBLIC void MCMod_SetThrusterValue(ThrusterID_t id, uint8_t input){
	SerialDebug(TAG, " Setting thruster ",id," to input ",input);
	DA_SetThrusterValue(id, MapInputToPWM(input, GetThrusterScalePWM(id), GetThrusterMinPWM(id)));
}

//takes uint8_t* and calls MCMod_SetThrusterValue with uint8_t id, uint8_t input
PUBLIC void MCMod_ThrusterCallback(uint8_t *data)
{
	MCMod_SetThrusterValue(data[0], data[1]);
	PiComms_Send("#ACK:",TAG,"!");
}

