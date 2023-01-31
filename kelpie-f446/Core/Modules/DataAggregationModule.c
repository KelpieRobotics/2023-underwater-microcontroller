/*
 * DataAggregationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "DataAggregationModule.h"
#include "SerialDebugDriver.h"

SystemData_t SystemData;

void DataAggregatorInit(){
}

PUBLIC void DA_SetThrusterValue(uint8_t thrusterId, pwm_t value){
	//SerialPrintln("DA_SetThrusterValue: thrusterId: %d, set value: %d, passed value: %d", thrusterId, SystemData.thrusterData[thrusterId].value, value);
	SystemData.thrusterData[thrusterId].value = value;
}

PUBLIC pwm_t DA_GetThrusterValue(uint8_t thrusterId){
	//SerialPrintln("DA_GetThrusterValue: thrusterId: %d, get value: %d", thrusterId, SystemData.thrusterData[thrusterId].value);
	return SystemData.thrusterData[thrusterId].value;
}

PUBLIC void DA_SetAppendageState(uint8_t appendageId, GPIO_PinState state){
	//SerialPrintln("DA_SetAppendageState: appendageId: %d, set state: %d, passed state: %d", appendageId, SystemData.actuatorData[appendageId].state, state);
	SystemData.actuatorData[appendageId].state = state;
}

PUBLIC GPIO_PinState DA_GetAppendageState(uint8_t appendageId){
	//SerialPrintln("DA_GetAppendageState: appendageId: %d, value: %d", appendageId, SystemData.actuatorData[appendageId].state);
	return SystemData.actuatorData[appendageId].state;
}

