/*
 * DataAggregationModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef MODULES_DATAAGGREGATIONMODULE_H_
#define MODULES_DATAAGGREGATIONMODULE_H_

#include "UserTypes.h"
#include "SerialDebugDriver.h"

typedef struct ThrusterData_t{
	pwm_t value;
} ThrusterData_t;

typedef struct ActuatorData_t{
	GPIO_PinState state;
} ActuatorData_t;

typedef struct IMUData_t{

} IMUData_t;

typedef struct PMBusData_t{

} PMBusData_t;

typedef struct SystemData_t{
	ThrusterData_t *thrusterData;
	ActuatorData_t *actuatorData;
	IMUData_t imuData;
	temperature_t temperature;
	PMBusData_t pmbusData;
	pressure_t pressure;


} SystemData_t;


PUBLIC void DA_SetThrusterValue(uint8_t thrusterId, pwm_t value);
PUBLIC pwm_t DA_GetThrusterValue(uint8_t thrusterId);
PUBLIC void DA_SetAppendageValue(uint8_t appendageId, GPIO_PinState value);
PUBLIC GPIO_PinState DA_GetAppendageValue(uint8_t appendageId);


#endif /* MODULES_DATAAGGREGATIONMODULE_H_ */
