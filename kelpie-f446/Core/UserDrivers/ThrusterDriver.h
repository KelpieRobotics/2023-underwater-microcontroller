/*
 * ThrusterDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERDRIVERS_THRUSTERDRIVER_H_
#define USERDRIVERS_THRUSTERDRIVER_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"

const uint8_t NUM_THRUSTERS = 8;
const pwm_t THRUSTER_SAFE_MIN_VALUE = 1250;		//this many need to be updated if we are using different thrusters
const pwm_t THRUSTER_SAFE_MAX_VALUE = 1750;
const pwm_t THRUSTER_INIT_VALUE = 1500;

typedef enum {
	THRUSTER1,
	THRUSTER2,
	THRUSTER3,
	THRUSTER4,
	THRUSTER5,
	THRUSTER6,
	THRUSTER7,
	THRUSTER8
} ThrusterID_t;

typedef struct
{
	ThrusterID_t id;
	pwm_t absMinPWM; // absolute min and max
	pwm_t absMaxPWM;

	pwm_t zeroPWM; // pwm for zero movement
	pwm_t safeMinPWM; // safe range for operation
	pwm_t safeMaxPWM;

	double counterValuePerPWM;
	uint8_t timerChannel; // hardware define
	TIM_HandleTypeDef * timerHandler;
	volatile uint32_t * timerReg;


} ThrusterInfo_t;

PUBLIC void ThrusterDriverInit();
PUBLIC void SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm);

#endif /* USERDRIVERS_THRUSTERDRIVER_H_ */
