/*
 * ServoDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef USERDRIVERS_SERVODRIVER_H_
#define USERDRIVERS_SERVODRIVER_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"

#define NUMBER_SERVOS 2

typedef enum {
	CLAW_ID = 0,
	WRIST_ID
} ServoID_t;

typedef struct ServoInfo{
	ServoID_t index;
	pwm_t maxPWMValue; // max pwm that can be sent to servo
	pwm_t minPWMValue; // min pwm that can be sent to servo
	pwm_t neutralPWMValue; // pwm value that sets servo to position "zero"
	pwm_t PWMPerDegree; // pwm value to move servo by 1 degree

	uint8_t timerChannel; // hardware define		// Appendages that will be swapped onto the same Channel will have the same channel here
	TIM_HandleTypeDef * timerHandler;
	volatile uint32_t * timerReg;
} ServoInfo_t;




PUBLIC result_t SetServoAngle(ServoID_t servoID, pwm_t targetAngle);
PUBLIC result_t ServoDriverInit();
PUBLIC pwm_t ServoDriverInputToPWM(uint8_t id, uint8_t input);

#endif /* USERDRIVERS_SERVODRIVER_H_ */
