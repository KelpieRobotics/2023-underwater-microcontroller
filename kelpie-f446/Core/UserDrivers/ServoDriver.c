/*
 * ServoDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "ServoDriver.h"
#include "SerialDebugDriver.h"

const SERVO_SCALE = (1 / 255);

extern TIM_HandleTypeDef htim1;

/*********************************************************************************
 *
 * 		Look up table for all Servos on the ROV
 *
 **********************************************************************************/
const ServoInfo_t servoLookup[NUMBER_SERVOS] =
{
		// IDs (Lookup Index)		Max PWM			Min PWM			Neutral PWM			PWM/Degree		TIMER CHANNEL		TIMER HANDLER		TIMER REG
		{CLAW_ID,					0xFFFF, 		0x0000,			0xBEEF,				0x0001,			TIM_CHANNEL_1,			&htim1,			&(TIM1->CCR1)},
		{WRIST_ID,					0xFFFF,			0x0000,			0xBEEF,				0x0001, 		TIM_CHANNEL_2,			&htim1,			&(TIM1->CCR2)}
};

// converts target angle into pwm value, checks if the pwm value is within range, send the pwm to the servo
// NOTE: it is the responsiblity of the module/user/appliction to updates the value in data aggregator, driver does not update the data aggr
PUBLIC result_t SetServoAngle(ServoID_t servoID, pwm_t targetAngle){
	*(servoLookup[servoID].timerReg) = targetAngle * servoLookup[servoID].PWMPerDegree;
	SerialPrintln("targetAngle as PWM: %d", *(servoLookup[servoID].timerReg));
	if(HAL_TIM_PWM_Start(servoLookup[servoID].timerHandler, servoLookup[servoID].timerChannel) == HAL_OK){
		return RESULT_OK;
	}
	return RESULT_ERR;

}


// Motor initialization sequence
PUBLIC result_t ServoDriverInit()
{
	uint8_t servoNum = NUMBER_SERVOS;
	result_t ret = RESULT_OK;

	while(servoNum > 0){
		if(SetServoAngle(servoNum--, servoLookup[servoNum].neutralPWMValue) != HAL_OK){
			ret = RESULT_ERR;
		}
	}
	return ret;
}

/*
 * Converts AppendageActuationModule input to ServoDriver PWM based on which Servo the input is for
 */
PUBLIC pwm_t ServoDriverInputToPWM(uint8_t id, uint8_t input){
	return (pwm_t)(servoLookup[id].minPWMValue + (servoLookup[id].maxPWMValue - servoLookup[id].minPWMValue) * (pwm_t)input * SERVO_SCALE);
}
