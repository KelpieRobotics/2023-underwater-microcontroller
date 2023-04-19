/*
 * SafetyManagerModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "SafetyManagerModule.h"
#include "LeakSensorDriver.h"
#include "SHT30DDriver.h"
#include "PressureTransducerDriver.h"
#include "PMBusDriver.h"
#include "SHT30DDriver.h"
#include "DataAggregationModule.h"
#include "SerialDebugDriver.h"
#include <UserTypes.h>

#define TAG "SMM"

PUBLIC result_t safetySensorsInit()
{
	InitLeakSensor();
	return RESULT_OK;
}

PUBLIC result_t updateSafetySensorRoutine()
{
	temperature_t shtTemp = 0;
	humidity_t shtHum = 0;
	if(SHT30D_GetValues(&shtTemp, &shtHum) == RESULT_ERR)	//THIS COMPARISON IS BETWEEN 2 DIFFERENT ERROR TYPES. I THINK THIS WAS TESTED, SO I WILL ASK THE AUTHOR TO TAKE A LOOK - Eric E
	{
		SerialDebug(TAG, "Error getting SHT30D values");
		PiComms_Send("#ERR:",TAG,"ErrorGettingSHT30DValues!");
		return RESULT_ERR;
	}

	SerialDebug(TAG, "Temp: %f\tHumidity: %f", shtTemp, shtHum);
	DA_SetHumidity(shtHum);
	DA_SetTemperature(shtTemp);
	return RESULT_OK;
}

/*
 * Sends "#ERR:SMM,LeakDetected!" when a leak is detected
 */
PUBLIC void SMModLeakSensorERR(){
	SerialDebug(TAG, "LeakDetected");
	PiComms_Send("#ERR:",TAG,",LeakDetected!");
}
