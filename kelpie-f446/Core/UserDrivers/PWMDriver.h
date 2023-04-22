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
#define THRUSTER_ABS_MIN_VALUE 1000		//this many need to be updated if we are using different thrusters
#define THRUSTER_ABS_MAX_VALUE 2000//I THOUGHT THE RANGE WAS 1100-1900. SAFE VALUES ARE USED, SO IT ISN'T CRITICAL  - ERIC E
#define THRUSTER_SAFE_MIN_VALUE 1100
#define THRUSTER_SAFE_MAX_VALUE 1900
#define THRUSTER_INIT_VALUE 1500
#define THRUSTER_COUNTER_TO_PWM_VALUE 1/0.0381
#define THRUSTER_PWM_SCALE (THRUSTER_SAFE_MAX_VALUE-THRUSTER_SAFE_MIN_VALUE)/255.0

#define NUM_SERVOS 3
#define SERVO_SAFE_MIN_VALUE 500		//this many need to be updated if we are using different thrusters
#define SERVO_SAFE_MAX_VALUE 2495
#define SERVO_SAFE_MIN_VALUE 500
#define SERVO_SAFE_MAX_VALUE 2495
#define SERVO_INIT_VALUE 1500
#define SERVO_COUNTER_TO_PWM_VALUE 1/0.0381
#define SERVO_PWM_SCALE (SERVO_SAFE_MAX_VALUE-SERVO_SAFE_MIN_VALUE)/255.0

#define LIGHT_MIN_VALUE 1100		//this many need to be updated if we are using different lights
#define LIGHT_MAX_VALUE 1900
#define LIGHT_COUNTER_TO_PWM_VALUE 1/0.0381
#define LIGHT_PWM_SCALE (LIGHT_MAX_VALUE-LIGHT_MIN_VALUE)/255.0

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

	float scalePWM;		//(safeMaxPWM - safeMinPWM)/255			safeMinPWM + 0-255 * scalePWM   converts uint8_t to pwm value

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

	float scalePWM;		//(safeMaxPWM - safeMinPWM)/255			safeMinPWM + 0-255 * scalePWM   converts uint8_t to pwm value

	double counterValuePerPWM;
	uint8_t timerChannel; // hardware define
	TIM_HandleTypeDef * timerHandler;
	volatile uint32_t * timerReg;


} PWMServoInfo_t;

PUBLIC pwm_t GetThrusterZeroValue(ThrusterID_t thrusterID);
PUBLIC result_t SetLightModulePWM(pwm_t pwm);
PUBLIC result_t SetServoPWM(PWMServoID_t SID, pwm_t pwm);
PUBLIC result_t SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm);
PUBLIC result_t PWMDriverThrustersInit();
PUBLIC result_t PWMDriverAppendagesInit();
PUBLIC pwm_t GetThrusterMinPWM(ThrusterID_t thrusterID);
PUBLIC const double GetThrusterScalePWM(ThrusterID_t thrusterID);
PUBLIC pwm_t GetServoMinPWM(PWMServoID_t servoID);
PUBLIC const double GetServoScalePWM(PWMServoID_t servoID);

#endif /* USERDRIVERS_THRUSTERDRIVER_H_ */
