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
#include "InterCommsTask.h"

#define TAG "MCM"
#define HORIZONTAL_RESPONDER 0		//we send sensor data to pi when thruster values are changed
#define VERTICAL_RESPONDER 4		//vertical and horizontal thrusters tend to act in groups

/*
 * converts the input to thruster pwm value, then calls DA_SetThrusterValue
 */
PUBLIC void MCMod_SetThrusterValue(ThrusterID_t id, uint8_t input){
	//SerialDebug(TAG, " Setting thruster ",id," to input ",input);
	DA_SetThrusterValue(id, MapInputToPWM(input, GetThrusterScalePWM(id), GetThrusterMinPWM(id)));
}

//takes uint8_t* and calls MCMod_SetThrusterValue with uint8_t id, uint8_t input
PUBLIC void MCMod_ThrusterCallback(uint8_t *data)
{
	MCMod_SetThrusterValue(data[0], data[1]);
	PiComms_Send("#ACK: %s !", TAG);
	if(data[0] == HORIZONTAL_RESPONDER || data[0] == VERTICAL_RESPONDER) ICommsTransmitRoutine();		//send sensor data to pi
}

