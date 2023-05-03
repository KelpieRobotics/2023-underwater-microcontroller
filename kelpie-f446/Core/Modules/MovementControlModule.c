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
PUBLIC void MCMod_ThrusterCallback(ThrusterCommand tc)
{
	MCMod_SetThrusterValue(0, tc.thruster_0_PWM);
	MCMod_SetThrusterValue(1, tc.thruster_1_PWM);
	MCMod_SetThrusterValue(2, tc.thruster_2_PWM);
	MCMod_SetThrusterValue(3, tc.thruster_3_PWM);
	MCMod_SetThrusterValue(4, tc.thruster_4_PWM);
	MCMod_SetThrusterValue(5, tc.thruster_5_PWM);
	MCMod_SetThrusterValue(6, tc.thruster_6_PWM);
	MCMod_SetThrusterValue(7, tc.thruster_7_PWM);

	PiComms_Send("#ACK:%s!", TAG);
}

