/*
 * ThrusterDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef USERDRIVERS_THRUSTERDRIVER_H_
#define USERDRIVERS_THRUSTERDRIVER_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"

#define NUM_THRUSTERS 8
#define THRUSTER_SAFE_MIN_VALUE 1250		//this many need to be updated if we are using different thrusters
#define THRUSTER_SAFE_MAX_VALUE 1750
#define THRUSTER_INIT_VALUE 1500

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

typedef enum {
	SERVO1,
	SERVO2,
	SERVO3
} PWMServoID_t;

typedef struct
{
	PWMServoID_t id;
	pwm_t absMinPWM; // absolute min and max
	pwm_t absMaxPWM;

	pwm_t zeroPWM; // pwm for zero movement
	pwm_t safeMinPWM; // safe range for operation
	pwm_t safeMaxPWM;

	double counterValuePerPWM;
	uint8_t timerChannel; // hardware define
	TIM_HandleTypeDef * timerHandler;
	volatile uint32_t * timerReg;


} PWMServoInfo_t;

PUBLIC result_t SetLightModulePWM(pwm_t pwm);
PUBLIC result_t SetServoPWM(PWMServoID_t SID, pwm_t pwm);
PUBLIC result_t SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm);
PUBLIC result_t ThrusterDriverInit();
PUBLIC pwm_t GetThrusterInitValue();

#endif /* USERDRIVERS_THRUSTERDRIVER_H_ */
