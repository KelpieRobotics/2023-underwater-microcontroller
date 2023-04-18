/*
 * NavigationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "NavigationModule.h"
#include "Adafruit_BNO080/IMUDriver.h"
#include "MS5837Driver.h"
#include "DataAggregationModule.h"
#include "SerialDebugDriver.h"

#define TAG "NAM"

PUBLIC result_t NAMod_SensorsInit()
{
	// Initialize Pressure Sensor
	if (MS5837_init() != RESULT_OK)
	{
		SerialDebug(TAG, "Failed to initialize PRESSURE SENSOR.");
		return RESULT_ERR;
	}

	// Initialize IMU (TBD)

	return RESULT_OK;
}

PUBLIC result_t NAMod_SensorRoutine()
{
	// Initialize pointers to variables
	depth_t depth;
	pressure_t pressure;

	result_t result;

	result = MS5837_getValues(&depth, &pressure);

	if (result != RESULT_OK)
	{
		SerialDebug(TAG, "Failed to get values from PRESSURE SENSOR.");
		return result;
	}

	SerialDebug(TAG, "Depth: %f\tPressure: %f", depth, pressure);
	DA_SetDepth(depth);
	DA_SetPressure(pressure);

	return RESULT_OK;
}
