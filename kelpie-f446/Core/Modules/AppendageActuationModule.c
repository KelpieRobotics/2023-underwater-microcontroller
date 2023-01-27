/*
 * AppendageActuationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "AppendageActuationModule.h"
#include "ServoDriver.h"
#include "DataAggregationModule.h"
#include "UserTypes.h"


PUBLIC AAMod_SetAppendageValue(uint8_t appendageID, appendage_val appen_val){
	//Will probably need a conversion function if appen_val is not passed in as uint8_t
	DA_SetAppendageValue(appendageID, (uint8_t)appen_val);
}

