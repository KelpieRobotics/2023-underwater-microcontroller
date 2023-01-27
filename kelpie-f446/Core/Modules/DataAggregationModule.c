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

PUBLIC void DA_SetThrusterValue(uint8_t thruster, pwm_t value){
	SystemData.thrusterData[thruster].value = value;
}

PUBLIC pwm_t DA_GetThrusterValue(uint8_t thrusterId){
	return SystemData.thrusterData[thrusterId].value;
}

PUBLIC void DA_SetAppendageValue(uint8_t appendage, appendage_t value){
	SystemData.actuatorData[appendage].value = value;
}

PUBLIC appendage_t DA_GetAppendageValue(uint8_t appendage){
	return SystemData.actuatorData[appendage].value;
}

