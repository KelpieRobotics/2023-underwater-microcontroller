/*
 * DataAggregationModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef MODULES_DATAAGGREGATIONMODULE_H_
#define MODULES_DATAAGGREGATIONMODULE_H_

#include "UserTypes.h"

typedef struct ThrusterData_t{
	uint8_t value;
} ThrusterData_t;

typedef struct ActuatorData_t{

} ActuatorData_t;

typedef struct IMUData_t{

} IMUData_t;

typedef struct PMBusData_t{

} PMBusData_t;

typedef struct SystemData_t{
	ThrusterData_t *thrusterData;
	ActuatorData_t actuatorData;
	IMUData_t imuData;
	temperature_t temperature;
	PMBusData_t pmbusData;
	pressure_t pressure;


} SystemData_t;

void SetThrusterPower(uint8_t thruster, uint8_t value);
uint8_t GetThrusterPower(uint8_t thrusterId);

#endif /* MODULES_DATAAGGREGATIONMODULE_H_ */
