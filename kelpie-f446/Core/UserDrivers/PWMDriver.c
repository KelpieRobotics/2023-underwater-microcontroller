/*
 * ThrusterDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "PWMDriver.h"
#include "SerialDebugDriver.h"
#include "DataAggregationModule.h"

#define TAG "PWM"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

ThrusterInfo_t thrusterLookup[NUM_THRUSTERS] =
{
		// ID			ABS MIN						ABS MAX						INIT						SAFE MIN						SAFE MAX 						SCALE					COUNTER TO PWM VALUE						TIMER CHANNEL		TIMER HANDLER		TIMER REG
		{THRUSTER1, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_1,			&htim8,			&(TIM8->CCR1)},
		{THRUSTER2, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_2,			&htim8,			&(TIM8->CCR2)},
		{THRUSTER3, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_3,			&htim8,			&(TIM8->CCR3)},
		{THRUSTER4, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_4,			&htim8,			&(TIM8->CCR4)},
		{THRUSTER5, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_1,			&htim1,			&(TIM1->CCR1)},
		{THRUSTER6, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_2,			&htim1,			&(TIM1->CCR2)},
		{THRUSTER7, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_3,			&htim1,			&(TIM1->CCR3)},
		{THRUSTER8, 	THRUSTER_ABS_MIN_VALUE,		THRUSTER_ABS_MAX_VALUE,		THRUSTER_INIT_VALUE,		THRUSTER_SAFE_MIN_VALUE,		THRUSTER_SAFE_MAX_VALUE,		THRUSTER_PWM_SCALE,		THRUSTER_COUNTER_TO_PWM_VALUE,				TIM_CHANNEL_4,			&htim1,			&(TIM1->CCR4)}
};

PUBLIC pwm_t GetThrusterZeroValue(ThrusterID_t thrusterID)
{
	return thrusterLookup[thrusterID].zeroPWM;
}

#define LIGHTMODULE 0
PWMServoInfo_t pwmServoLookup[NUM_SERVOS] =
{
		// ID			ABS MIN						ABS MAX						ZERO						SAFE MIN					SAFE MAX 						SCALE					COUNTER TO PWM VALUE				TIMER CHANNEL		TIMER HANDLER		TIMER REG
		{SERVO1, 		SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,		SERVO_INIT_VALUE,			SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,			SERVO_PWM_SCALE,		SERVO_COUNTER_TO_PWM_VALUE,			TIM_CHANNEL_1,			&htim3,			&(TIM3->CCR1)},
		{SERVO2, 		SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,		SERVO_INIT_VALUE,			SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,			SERVO_PWM_SCALE,		SERVO_COUNTER_TO_PWM_VALUE,			TIM_CHANNEL_2,			&htim3,			&(TIM3->CCR2)},
		{SERVO3, 		SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,		SERVO_INIT_VALUE,			SERVO_SAFE_MIN_VALUE,		SERVO_SAFE_MAX_VALUE,			SERVO_PWM_SCALE,		SERVO_COUNTER_TO_PWM_VALUE,			TIM_CHANNEL_3,			&htim3,			&(TIM3->CCR3)}
};

PUBLIC result_t SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm)
{

	*(thrusterLookup[thrusterID].timerReg) = pwm * thrusterLookup[thrusterID].counterValuePerPWM;
	if(HAL_TIM_PWM_Start(thrusterLookup[thrusterID].timerHandler, thrusterLookup[thrusterID].timerChannel) == HAL_OK){
		return RESULT_OK;
	}
	return RESULT_ERR;
}

PUBLIC result_t SetServoPWM(PWMServoID_t SID, pwm_t pwm)
{
	SerialDebug(TAG, "ID: %d PWM: %d", SID, pwm);
	*(pwmServoLookup[SID].timerReg) = pwm * pwmServoLookup[SID].counterValuePerPWM;
		if(HAL_TIM_PWM_Start(pwmServoLookup[SID].timerHandler, pwmServoLookup[SID].timerChannel) == HAL_OK){
			return RESULT_OK;
		}
		return RESULT_ERR;
}

PUBLIC result_t SetLightModulePWM(pwm_t pwm)
{
	SerialDebug(TAG, "Light PWM: %d", pwm);
	TIM3->CCR4 = pwm * LIGHT_COUNTER_TO_PWM_VALUE;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	return RESULT_ERR;
}

// Motor initialization sequence
PUBLIC result_t PWMDriverThrustersInit()
{
	result_t ret = RESULT_OK;
	//initialize thrusters
	uint8_t n = NUM_THRUSTERS;
	while(n > 0){
		--n;
		if(SetThrusterPWM(n, thrusterLookup[n].zeroPWM) != RESULT_OK){
			ret = RESULT_ERR;
		}
	}

	return ret;
}

// Motor initialization sequence
PUBLIC result_t PWMDriverAppendagesInit()
{
	result_t ret = RESULT_OK;
	//initialize servos
	uint8_t n = NUM_SERVOS;
	while(n > 0){
		--n;
		if(SetServoPWM(n, pwmServoLookup[n].zeroPWM) != RESULT_OK){
			ret = RESULT_ERR;
		}
	}

	//initialize light to off
	if(SetLightModulePWM(LIGHT_MIN_VALUE) != RESULT_OK){
		ret = RESULT_ERR;
	}

	return ret;
}

PUBLIC pwm_t GetThrusterMinPWM(ThrusterID_t thrusterID){
	return thrusterLookup[thrusterID].safeMinPWM;
}
PUBLIC const double GetThrusterScalePWM(ThrusterID_t thrusterID){
	return thrusterLookup[thrusterID].scalePWM;
}

PUBLIC pwm_t GetServoMinPWM(PWMServoID_t servoID){
	return pwmServoLookup[servoID].safeMinPWM;
}
PUBLIC const double GetServoScalePWM(PWMServoID_t servoID){
	return pwmServoLookup[servoID].scalePWM;
}
