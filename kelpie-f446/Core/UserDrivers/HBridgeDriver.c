/*
 * ServoDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include <HBridgeDriver.h>
#include "SerialDebugDriver.h"

extern TIM_HandleTypeDef htim2;
//extern HAL_ADC_HandleTypeDef hadc1;

#define TAG "HBD"

/*********************************************************************************
 *
 * 		Look up table for all Servos on the ROV
 *
 **********************************************************************************/
const HBridgeInfo_t hBridgeLookup[NUM_HBRIDGE] =
{
		// IDs (Lookup Index)		neutralPinValue			GPIO_Pin			GPIO Bank
		{CLAW_L,					GPIO_PIN_RESET,			GPIO_PIN_10,		GPIOC},
		{CLAW_R,					GPIO_PIN_RESET,			GPIO_PIN_5, 		GPIOC}
};

// converts target angle into pwm value, checks if the pwm value is within range, send the pwm to the servo
// NOTE: it is the responsiblity of the module/user/appliction to updates the value in data aggregator, driver does not update the data aggr
PUBLIC void SetHBridgeValue(ServoID_t servoID, GPIO_PinState state)
{
	SerialDebug(TAG, "ID: %d State: %d", servoID, state);
	HAL_GPIO_WritePin(hBridgeLookup[servoID].GPIO_Bank, hBridgeLookup[servoID].GPIO_Pin, state);
}

// Motor initialization sequence
PUBLIC void HBridgeDriverInit()
{

	uint8_t servoNum = NUM_HBRIDGE;

	while(servoNum > 0){

		SetHBridgeValue(servoNum, hBridgeLookup[servoNum].neutralPinValue);
		servoNum--;
	}
}
