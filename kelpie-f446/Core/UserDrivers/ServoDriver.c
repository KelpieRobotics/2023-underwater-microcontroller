/*
 * ServoDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "ServoDriver.h"
#include "SerialDebugDriver.h"

extern TIM_HandleTypeDef htim2;
//extern HAL_ADC_HandleTypeDef hadc1;



/*********************************************************************************
 *
 * 		Look up table for all Servos on the ROV
 *
 **********************************************************************************/
const ServoInfo_t servoLookup[NUMBER_SERVOS] =
{
		// IDs (Lookup Index)		neutralPinValue			GPIO_Pin			GPIO Bank
		{CLAW_L,					GPIO_PIN_RESET,			GPIO_PIN_14,		GPIOB},
		{CLAW_R,					GPIO_PIN_RESET,			GPIO_PIN_15, 		GPIOB}
};

// converts target angle into pwm value, checks if the pwm value is within range, send the pwm to the servo
// NOTE: it is the responsiblity of the module/user/appliction to updates the value in data aggregator, driver does not update the data aggr
PUBLIC void SetServoAngle(ServoID_t servoID, GPIO_PinState state){
	SerialPrintln("Pin State: %d", state);

	HAL_GPIO_WritePin(servoLookup[servoID].GPIO_Bank, servoLookup[servoID].GPIO_Pin, state);
}

// Motor initialization sequence
PUBLIC void ServoDriverInit()
{

	uint8_t servoNum = NUMBER_SERVOS;

	while(servoNum > 0){

		SetServoAngle(servoNum, servoLookup[servoNum].neutralPinValue);
		servoNum--;
	}
}
