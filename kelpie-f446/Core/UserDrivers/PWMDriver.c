/*
 * ThrusterDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "PWMDriver.h"
#include "SerialDebugDriver.h"
#include "DataAggregationModule.h"
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

ThrusterInfo_t thrusterLookup[NUM_THRUSTERS] =
{
		// ID			ABS MIN		ABS MAX			ZERO		SAFE MIN	SAFE MAX 	COUNTER TO PWM VALUE		TIMER CHANNEL		TIMER HANDLER		TIMER REG
		{THRUSTER1, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_1,			&htim8,			&(TIM8->CCR1)},
		{THRUSTER2, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_2,			&htim8,			&(TIM8->CCR2)},
		{THRUSTER3, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_3,			&htim8,			&(TIM8->CCR3)},
		{THRUSTER4, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_4,			&htim8,			&(TIM8->CCR4)},
		{THRUSTER5, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_1,			&htim1,			&(TIM1->CCR1)},
		{THRUSTER6, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_2,			&htim1,			&(TIM1->CCR2)},
		{THRUSTER7, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_3,			&htim1,			&(TIM1->CCR3)},
		{THRUSTER8, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_4,			&htim1,			&(TIM1->CCR4)}
};

#define LIGHTMODULE 0
PWMServoInfo_t pwmServoLookup[3] =
{
		// ID			ABS MIN		ABS MAX			ZERO		SAFE MIN	SAFE MAX 	COUNTER TO PWM VALUE		TIMER CHANNEL		TIMER HANDLER		TIMER REG
		{SERVO1, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_1,			&htim3,			&(TIM3->CCR1)},
		{SERVO2, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_2,			&htim3,			&(TIM3->CCR2)},
		{SERVO3, 		1000,		2000,		1500,		1250,		1750,			1/0.0381,				TIM_CHANNEL_3,			&htim3,			&(TIM3->CCR3)}
};

PUBLIC result_t SetLightModulePWM(pwm_t pwm)
{
	TIM3->CCR4 = pwm * (1/0.0381);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	return RESULT_ERR;
}

PUBLIC result_t SetServoPWM(PWMServoID_t SID, pwm_t pwm)
{
	*(pwmServoLookup[SID].timerReg) = pwm * pwmServoLookup[SID].counterValuePerPWM;
		if(HAL_TIM_PWM_Start(pwmServoLookup[SID].timerHandler, pwmServoLookup[SID].timerChannel) == HAL_OK){
			return RESULT_OK;
		}
		return RESULT_ERR;
}

PUBLIC result_t SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm)
{

	*(thrusterLookup[thrusterID].timerReg) = pwm * thrusterLookup[thrusterID].counterValuePerPWM;
	if(HAL_TIM_PWM_Start(thrusterLookup[thrusterID].timerHandler, thrusterLookup[thrusterID].timerChannel) == HAL_OK){
		return RESULT_OK;
	}
	return RESULT_ERR;
}

// Motor initialization sequence
PUBLIC result_t ThrusterDriverInit()
{
	uint8_t numThrusters = NUM_THRUSTERS;
	result_t ret = RESULT_OK;
	while(numThrusters > 0){
		numThrusters--;
		if(SetThrusterPWM(numThrusters, THRUSTER_INIT_VALUE) != RESULT_OK){
			ret = RESULT_ERR;
		}
	}
	return ret;
}

PUBLIC pwm_t GetThrusterInitValue(){
	return THRUSTER_INIT_VALUE;
}
