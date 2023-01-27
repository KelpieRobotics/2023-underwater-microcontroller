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

PUBLIC void DA_SetThrusterValue(uint8_t thruster, uint8_t value){
	SystemData.thrusterData[thruster].value;
}

PUBLIC uint8_t DA_GetThrusterValue(uint8_t thrusterId){
	return SystemData.thrusterData[thrusterId].value;
}

