/*
 * AppendageActuationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "AppendageActuationModule.h"
#include "DataAggregationModule.h"
#include "ServoDriver.h"
#include "UserTypes.h"

/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void AAMod_SetAppendageValue(ServoID_t id, uint8_t input){
	DA_SetThrusterPower(id, ServoDriverInputToPWM(id, input));
}


