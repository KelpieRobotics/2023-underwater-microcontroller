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
	SystemData = calloc(sizeof(SystemData_t));
	SystemData.thrusterData = calloc(sizeof(ThrusterData_t) * NUM_THRUSTERS);
}

void DataAggregatorUninit(){
	free(SystemData.thrusterData);
	free(SystemData);
}

PUBLIC void SetThrusterPower(uint8_t thruster, uint8_t value){
	SystemData.thrusterData[thruster].value;
}

PUBLIC uint8_t GetThrusterPower(uint8_t thrusterId){
	return SystemData.thrusterData[thrusterId].value;
}

