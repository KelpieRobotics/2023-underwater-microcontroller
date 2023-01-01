/*
 * ServoDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERDRIVERS_SERVODRIVER_H_
#define USERDRIVERS_SERVODRIVER_H_

#include "UserTypes.h"

#define NUMBER_SERVOS 2

typedef enum {
	CLAW_ID = 0,
	WRIST_ID
} ServoID;

typedef struct ServoInfo{
	ServoID index;
	pwm_t maxPWMValue; // max pwm that can be sent to servo
	pwm_t minPWMValue; // min pwm that can be sent to servo
	pwm_t neutralPWMValue; // pwm value that sets servo to position "zero"
	pwm_t PWMPerDegree; // pwm value to move servo by 1 degree
} ServoInfo_t;

PUBLIC result_t InitServoDriver();
PUBLIC result_t SetServoAngle(ServoID id, angular_degrees_t targetAngle);

#endif /* USERDRIVERS_SERVODRIVER_H_ */
