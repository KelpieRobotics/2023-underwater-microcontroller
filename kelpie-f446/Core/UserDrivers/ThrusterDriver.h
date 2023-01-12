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
// PWM Counter registers
#define THRUSTER1_REG TIM8->CCR1
#define THRUSTER2_REG
#define THRUSTER3_REG
#define THRUSTER4_REG
#define THRUSTER5_REG
#define THRUSTER6_REG
#define THRUSTER7_REG
#define THRUSTER8_REG

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

	uint16_t counterValuePerPWM;
	uint8_t timerChannel; // hardware define
	TIM_HandleTypeDef * timerHandler;


} ThrusterInfo_t;

PUBLIC void ThrusterDriverInit();
PUBLIC void SetThrusterPWM(ThrusterID_t thrusterID, pwm_t pwm);

#endif /* USERDRIVERS_THRUSTERDRIVER_H_ */
