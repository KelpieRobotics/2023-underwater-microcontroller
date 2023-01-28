/*
 * DataAggregationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "DataAggregationModule.h"
#include "ThrusterDriver.h"

SystemData_t SystemData;

void DataAggregatorInit(){
}

PUBLIC void DA_SetThrusterValue(uint8_t thrusterId, pwm_t value){
	SystemData.thrusterData[thrusterId].value = value;
}

PUBLIC pwm_t DA_GetThrusterValue(uint8_t thrusterId){
	return SystemData.thrusterData[thrusterId].value;
}

PUBLIC void DA_SetAppendageValue(uint8_t appendageId, pwm_t value){
	SystemData.actuatorData[appendageId].value = value;
}

PUBLIC pwm_t DA_GetAppendageValue(uint8_t appendageId){
	return SystemData.actuatorData[appendageId].value;
}

