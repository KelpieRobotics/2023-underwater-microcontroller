/*
 * DataAggregationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "DataAggregationModule.h"
#include "SerialDebugDriver.h"

//#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DPRINT(...) SerialDebug(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

#define TAG "DAM"

SystemData_t SystemData;

void DataAggregatorInit()
{
}

PUBLIC void DA_SetThrusterValue(uint8_t thrusterId, pwm_t value)
{
	//SerialPrintln("DA_SetThrusterValue: thrusterId: %d, set value: %d, passed value: %d", thrusterId, SystemData.thrusterData[thrusterId].value, value);
	SystemData.thrusterData[thrusterId].value = value;
}
PUBLIC pwm_t DA_GetThrusterValue(uint8_t thrusterId)
{
	//SerialPrintln("DA_GetThrusterValue: thrusterId: %d, get value: %d", thrusterId, SystemData.thrusterData[thrusterId].value);
	return SystemData.thrusterData[thrusterId].value;
}

PUBLIC void DA_SetLightValue(pwm_t value)
{
	SystemData.lightData = value;
}
PUBLIC pwm_t DA_GetLightValue()
{
	return SystemData.lightData;
}

PUBLIC void DA_SetPWMServoValue(uint8_t pwmServoId, pwm_t value)
{
	SystemData.pwmServoData[pwmServoId].value = value;
}
PUBLIC pwm_t DA_GetPWMServoValue(uint8_t pwmServoId)
{
	return SystemData.pwmServoData[pwmServoId].value;
}

PUBLIC void DA_SetAppendageState(uint8_t appendageId, GPIO_PinState state)
{
	//SerialPrintln("DA_SetAppendageState: appendageId: %d, set state: %d, passed state: %d", appendageId, SystemData.actuatorData[appendageId].state, state);
	SystemData.actuatorData[appendageId].state = state;
}
PUBLIC GPIO_PinState DA_GetAppendageState(uint8_t appendageId)
{
	//SerialPrintln("DA_GetAppendageState: appendageId: %d, value: %d", appendageId, SystemData.actuatorData[appendageId].state);
	return SystemData.actuatorData[appendageId].state;
}

PUBLIC void DA_SetHumidity(humidity_t hum)
{
	DPRINT(TAG, "Humidity write");
	SystemData.humidity = hum;
	DPRINT(TAG, "H %f", hum);
}

PUBLIC void DA_SetTemperature(temperature_t t)
{
	DPRINT(TAG, "Temp write");
	SystemData.temperature = t;
}

PUBLIC void DA_SetDepth(depth_t t)
{
	DPRINT(TAG, "Depth write");
	SystemData.depth = t;
}

PUBLIC void DA_SetPressure(pressure_t t)
{
	DPRINT(TAG, "Pressure write");
	SystemData.pressure = t;
}

PUBLIC humidity_t DA_GetHumidity()
{
	DPRINT(TAG, "Humidity read");
	return SystemData.humidity;
}

PUBLIC temperature_t DA_GetTemperature()
{
	DPRINT(TAG, "Temp read");
	return SystemData.temperature;
}

PUBLIC depth_t DA_GetDepth()
{
	DPRINT(TAG, "Depth read");
	return SystemData.depth;
}

PUBLIC pressure_t DA_GetPressure()
{
	DPRINT(TAG, "Pressure read");
	return SystemData.pressure;
}

PUBLIC void DA_SetIMUQuaterion(float i, float j, float k, float real)
{
	DPRINT(TAG, "Quat write");
	SystemData.imuData.quat_i = i;
	SystemData.imuData.quat_j = j;
	SystemData.imuData.quat_k = k;
	SystemData.imuData.quat_real= real;
}

PUBLIC IMUData_t DA_GetIMUQuaterion()
{
	DPRINT(TAG, "Quat read");
	return SystemData.imuData;
}

