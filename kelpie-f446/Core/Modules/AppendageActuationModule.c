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
	SerialDebug(TAG, "AAMod_SetAppendageValue state: %d", state);
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

/*
 * converts the input to light pwm value, then calls DA_SetLightValue
 */
PUBLIC void AAMod_SetLightValue(uint8_t input){
	SerialDebug(TAG, " ----------Setting light to input %d",input);
	DA_SetLightValue(MapInputToPWM(input, LIGHT_PWM_SCALE, LIGHT_MIN_VALUE));
}

/*
 * converts the input to servo pwm value, then calls DA_SetPWMServoValue
 */
PUBLIC void AAMod_SetServoValue(ServoID_t id, uint8_t input){
	SerialDebug(TAG, " Setting servo %d to input %d", id, input);
	DA_SetPWMServoValue(id, MapInputToPWM(input, GetServoScalePWM(id), GetServoMinPWM(id)));
}

PUBLIC void AAMod_Callback(AttachmentCommand ac){
	AAMod_SetAppendageValue((claw_state_t)ac.claw_state);
	AAMod_SetLightValue((uint8_t)ac.light_PWM);
	AAMod_SetServoValue(0, ac.servo_0_PWM);
	AAMod_SetServoValue(1, ac.servo_1_PWM);
	AAMod_SetServoValue(2, ac.servo_2_PWM);

	PiComms_Send("#ACK:%s!", TAG);
}
