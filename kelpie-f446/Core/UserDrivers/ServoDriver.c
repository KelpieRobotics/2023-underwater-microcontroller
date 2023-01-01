/*
 * ServoDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "ServoDriver.h"

/*********************************************************************************
 *
 * 		Look up table for all Servos on the ROV
 *
 **********************************************************************************/
const ServoInfo_t ServoLookUpTable[NUMBER_SERVOS] =
	{
			// IDs (Lookup Index)		Max PWM			Min PWM			Neutral PWM			PWM/Degree
			{CLAW_ID,					0xFFFF, 		0x0000,			0xBEEF,				0x0001},
			{WRIST_ID,					0xFFFF,			0x0000,			0xBEEF,				0x0001}
	};


PUBLIC result_t InitServoDriver();

// converts target angle into pwm value, checks if the pwm value is within range, send the pwm to the servo
// NOTE: it is the responsiblity of the module/user/appliction to updates the value in data aggregator, driver does not update the data aggr
PUBLIC result_t SetServoAngle(ServoID id, angular_degrees_t targetAngle);
