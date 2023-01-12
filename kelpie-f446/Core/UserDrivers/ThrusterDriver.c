/*
 * ThrusterDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "ThrusterDriver.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim8;
ThrusterInfo_t thrusterLookup[8] =
{
		// ID			ABS MIN		ABS MAX			ZERO		SAFE MIN	SAFE MAX 	COUNTER TO PWM VALUE	TIMER CHANNEL		TIMER HANDLER
		{THRUSTER1, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_1,			&htim8,			&(TIM8->CCR1)},
		{THRUSTER2, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_2,			&htim8,			&(TIM8->CCR2)},
		{THRUSTER3, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_3,			&htim8,			&(TIM8->CCR3)},
		{THRUSTER4, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_4,			&htim8,			&(TIM8->CCR4)},
		{THRUSTER5, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_1,			&htim1,			&(TIM1->CCR1)},
		{THRUSTER6, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_2,			&htim1,			&(TIM1->CCR2)},
		{THRUSTER7, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_3,			&htim1,			&(TIM1->CCR3)},
		{THRUSTER8, 		1000,		2000,		1500,		1250,		1750,			1927,				TIM_CHANNEL_4,			&htim1,			&(TIM1->CCR4)}
};

PUBLIC void SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm)
{

	*(thrusterLookup[thrusterID].timerReg) = pwm * thrusterLookup[thrusterID].counterValuePerPWM;
	HAL_TIM_PWM_Start(thrusterLookup[thrusterID].timerHandler, thrusterLookup[thrusterID].timerChannel);
}

// Motor initialization sequence
PUBLIC void ThrusterDriverInit()
{

}
