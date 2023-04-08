/*
 * ServoDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#ifndef USERDRIVERS_HBRIDGEDRIVER_H_
#define USERDRIVERS_HBRIDGEDRIVER_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"

#define NUMBER_SERVOS 2

typedef enum {
	CLAW_L = 0,
	CLAW_R
} ServoID_t;

typedef struct ServoInfo{
	ServoID_t index;
	GPIO_PinState neutralPinValue; // pin value that sets servo to position "zero"
	uint16_t GPIO_Pin;
	GPIO_TypeDef *GPIO_Bank;
} ServoInfo_t;




PUBLIC void SetServoAngle(ServoID_t servoID, GPIO_PinState state);
PUBLIC void ServoDriverInit();
PUBLIC pwm_t ServoDriverInputToPWM(uint8_t id, uint8_t input);

#endif /* USERDRIVERS_HBRIDGEDRIVER_H_ */
