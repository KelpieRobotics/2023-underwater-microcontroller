/*
 * AppendageActuationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "AppendageActuationModule.h"
#include "DataAggregationModule.h"
#include "UserTypes.h"
#include <UserUtils.h>
#include "PiCommsDriver.h"

#define TAG "AAM"
/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void AAMod_SetAppendageValue(claw_state_t state){
	SerialDebug(TAG, "#DEBUG: AAMod_SetAppendageValue state: %d", state);
	switch(state){
	case CLAW_OPEN:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_SET);
		break;
	case CLAW_CLOSE:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_SET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_RESET);
		break;
	default:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_RESET);
	}
}
PUBLIC void AAMod_AppendageCallback(uint8_t * data){
	int8_t intData = (int)data[0];
	AAMod_SetAppendageValue((claw_state_t)intData);
	PiComms_Send("#ACK:",TAG,"!");
}


/*
 * converts the input to servo pwm value, then calls DA_SetPWMServoValue
 */
PUBLIC void AAMod_SetServoValue(ServoID_t id, uint8_t input){
	SerialDebug(TAG, " Setting servo ",id," to input ",input);
	DA_SetPWMServoValue(id, MapInputToPWM(input, GetServoScalePWM(id), GetServoMinPWM(id)));
}
//takes uint8_t* and calls MCMod_SetThrusterValue with uint8_t id, uint8_t input
PUBLIC void AAMod_ServoCallback(uint8_t *data){
	AAMod_SetServoValue(data[0], data[1]);
	PiComms_Send("#ACK:",TAG,"!");
}


/*
 * converts the input to light pwm value, then calls DA_SetLightValue
 */
PUBLIC void AAMod_SetLightValue(uint8_t input){
	SerialDebug(TAG, " Setting light to input ",input);
	DA_SetLightValue(MapInputToPWM(input, LIGHT_PWM_SCALE, LIGHT_MIN_VALUE));
}
//takes uint8_t* and calls MCMod_SetLightValue with uint8_t input
PUBLIC void AAMod_LightCallback(uint8_t *data){
	AAMod_SetLightValue(data[0]);
	PiComms_Send("#ACK:",TAG,"!");
}
