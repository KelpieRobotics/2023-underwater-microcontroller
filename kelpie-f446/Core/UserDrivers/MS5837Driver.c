/*
 * MS5837Driver.c
 *
 *  Created on: Apr 1, 2023
 *      Author: Adam
 *
 *  Adapted from: https://github.com/bluerobotics/BlueRobotics_MS5837_Library
 */


// STM headers
#include "stm32f4xx_hal.h"
// My Header
#include "MS5837Driver.h"
// Serial library
#include "SerialDebugDriver.h"
// Standard library
#include <stdbool.h>
#include <math.h>

#define SENSOR_TIMEOUT 500

extern I2C_HandleTypeDef hi2c1;

const uint8_t MS5837_ADDR = 0x76 << 1;
const uint8_t MS5837_RESET = 0x1E;
const uint8_t MS5837_ADC_READ = 0x00;
const uint8_t MS5837_PROM_READ = 0xA0;
const uint8_t MS5837_CONVERT_D1_8192 = 0x4A;
const uint8_t MS5837_CONVERT_D2_8192 = 0x5A;



const uint8_t MS5837_02BA01 = 0x00; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_02BA21 = 0x15; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_30BA26 = 0x1A; // Sensor version: From MS5837_30BA datasheet Version PROM Word 0


/**
 * @summary: Sets default fluid density to that of water
 */
void MS5837() {
	fluidDensity = 1029;
}

/**
 * @summary: Must be called before attempting to operate the sensor.
 * @return: result_t - RESULT_OK if successful, RESULT_ERR if not
 */
result_t init() // assign this function to the initialised boolean when you call it;
{
	HAL_StatusTypeDef ret;
	uint8_t buffer[6];

	buffer[0] = MS5837_RESET;
	ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
	{
		SerialPrintln("Reset failed.");
		return RESULT_ERR;
	}

	HAL_Delay(10);

	for (uint8_t i = 0 ; i < 7 ; i++) {
			buffer[0] = MS5837_PROM_READ+i*2;

			ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
			if (ret != HAL_OK)
			{
				SerialPrintln("Request failed.");
				return RESULT_ERR;
			}

			ret = HAL_I2C_Master_Receive(&hi2c1, MS5837_ADDR, buffer, 2, SENSOR_TIMEOUT);
			if (ret != HAL_OK)
			{
				SerialPrintln("Failed to receive.");
				return RESULT_ERR;
			}

			C[i] = (buffer[0] << 8) | buffer[1];
		}

	// Verify that data is correct with CRC
		uint8_t crcRead = C[0] >> 12;
		uint8_t crcCalculated = crc4(C);

		if ( crcCalculated != crcRead ) {
			SerialPrintln("Failed CRC.");
			return RESULT_ERR; // CRC fail
		}

		uint8_t version = (C[0] >> 5) & 0x7F; // Extract the sensor version from PROM Word 0

		// Set _model according to the sensor version
		if (version == MS5837_02BA01)
		{
			_model = MS5837_02BA;
		}
		else if (version == MS5837_02BA21)
		{
			_model = MS5837_02BA;
		}
		else if (version == MS5837_30BA26)
		{
			_model = MS5837_30BA;
		}
		else
		{
			_model = MS5837_UNRECOGNISED;
		}
		// The sensor has passed the CRC check, so we should return true even if
		// the sensor version is unrecognised.
		// (The MS5637 has the same address as the MS5837 and will also pass the CRC check)
		// (but will hopefully be unrecognised.)
		return RESULT_OK;
}

/**
 * @summary: Set model of MS5837 sensor, options are; MS5837_30BA and MS5837_02BA
 * @param: uint8_t model - model type
 */
void MS5837_setModel(uint8_t model) {
	_model = model;
}

/**
 * @summary: Provide the density of the working fluid in kg/m^3. Default is for
 * seawater. Should be 997 for freshwater
 * @param: float density - fluid density in kg/m^3
 */
void MS5837_setFluidDensity(float density)
{
	fluidDensity = density;
}

/**
 * @summary: Obtains the temperature and pressure data and sets the D1_pres and D2_temp vars accordingly
 * @return: result_t - RESULT_OK if successful, RESULT_ERR if not
 */
result_t MS5837_read()
{
	if(!initialised)
	{
		return RESULT_ERR;
	}

	HAL_StatusTypeDef ret;

	uint8_t buffer[6];
	buffer[0] = MS5837_CONVERT_D1_8192;

	// Request D1 conversion
	ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
	{
		SerialPrintln("Failed request for D1 conversion.");
		return RESULT_ERR;
	}

	vTaskDelay(20 / portTICK_PERIOD_MS);

	buffer[0] = MS5837_ADC_READ;
	ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
		{
			SerialPrintln("Failed request for D1 conversion.");
			return RESULT_ERR;
		}


	ret = HAL_I2C_Master_Receive(&hi2c1, MS5837_ADDR, buffer, 3, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
		{
			SerialPrintln("Failed request for D1 conversion.");
			return RESULT_ERR;
		}

	D1_pres = 0;
	D1_pres = buffer[0];
	D1_pres = (D1_pres << 8) | buffer[1];
	D1_pres = (D1_pres << 8) | buffer[2];

	// Request D2 conversion
	buffer[0] = MS5837_CONVERT_D2_8192;
	ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
		{
			SerialPrintln("Failed request for D1 conversion.");
			return RESULT_ERR;
		}

	vTaskDelay(20 / portTICK_PERIOD_MS); // max delay as per datasheet

	buffer[0] = MS5837_ADC_READ;
	ret = HAL_I2C_Master_Transmit(&hi2c1, MS5837_ADDR, buffer, 1, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
		{
			SerialPrintln("Failed request for D1 conversion.");
			return RESULT_ERR;
		}

	ret = HAL_I2C_Master_Receive(&hi2c1, MS5837_ADDR, buffer, 3, SENSOR_TIMEOUT);
	if (ret != HAL_OK)
		{
			SerialPrintln("Failed request for D1 conversion.");
			return RESULT_ERR;
		}

	D2_temp = 0;
	D2_temp = buffer[0];
	D2_temp = (D2_temp << 8) | buffer[1];
	D2_temp = (D2_temp << 8) | buffer[2];

	MS5837_calculate();

	return RESULT_OK;
}

/**
 * @summary: Performs calculations per the sensor data sheet for conversion and second order compensation.
 */
void MS5837_calculate()
{
	// Given C1-C6 and D1, D2, calculated TEMP and P
	// Do conversion first and then second order temp compensation

		int32_t dT = 0;
		int64_t SENS = 0;
		int64_t OFF = 0;
		int32_t SENSi = 0;
		int32_t OFFi = 0;
		int32_t Ti = 0;
		int64_t OFF2 = 0;
		int64_t SENS2 = 0;

		// Terms called
		dT = D2_temp-(uint32_t)C[5]*256l;
		if ( _model == MS5837_02BA ) {
			SENS = (int64_t)C[1]*65536l+((int64_t)C[3]*dT)/128l;
			OFF = (int64_t)C[2]*131072l+((int64_t)C[4]*dT)/64l;
			P = (D1_pres*SENS/(2097152l)-OFF)/(32768l);
		} else {
			SENS = (int64_t)C[1]*32768l+((int64_t)C[3]*dT)/256l;
			OFF = (int64_t)C[2]*65536l+((int64_t)C[4]*dT)/128l;
			P = (D1_pres*SENS/(2097152l)-OFF)/(8192l);
		}

		// Temp conversion
		TEMP = 2000l+(int64_t)dT*C[6]/8388608LL;

		//Second order compensation
		if ( _model == MS5837_02BA ) {
			if((TEMP/100)<20){         //Low temp
				Ti = (11*(int64_t)dT*(int64_t)dT)/(34359738368LL);
				OFFi = (31*(TEMP-2000)*(TEMP-2000))/8;
				SENSi = (63*(TEMP-2000)*(TEMP-2000))/32;
			}
		} else {
			if((TEMP/100)<20){         //Low temp
				Ti = (3*(int64_t)dT*(int64_t)dT)/(8589934592LL);
				OFFi = (3*(TEMP-2000)*(TEMP-2000))/2;
				SENSi = (5*(TEMP-2000)*(TEMP-2000))/8;
				if((TEMP/100)<-15){    //Very low temp
					OFFi = OFFi+7*(TEMP+1500l)*(TEMP+1500l);
					SENSi = SENSi+4*(TEMP+1500l)*(TEMP+1500l);
				}
			}
			else if((TEMP/100)>=20){    //High temp
				Ti = 2*(dT*dT)/(137438953472LL);
				OFFi = (1*(TEMP-2000)*(TEMP-2000))/16;
				SENSi = 0;
			}
		}

		OFF2 = OFF-OFFi;           //Calculate pressure and temp second order
		SENS2 = SENS-SENSi;

		TEMP = (TEMP-Ti);

		if ( _model == MS5837_02BA ) {
			P = (((D1_pres*SENS2)/2097152l-OFF2)/32768l);
		} else {
			P = (((D1_pres*SENS2)/2097152l-OFF2)/8192l);
		}

}

/**
 * @summary: returns pressure according to a conversion factor
 *	The pressure sensor measures absolute pressure, so it will measure the atmospheric pressure + water pressure
 * We subtract the atmospheric pressure to calculate the depth with only the water pressure
 * The average atmospheric pressure of 101300 pascal is used for the calcuation, but atmospheric pressure varies
 * If the atmospheric pressure is not 101300 at the time of reading, the depth reported will be offset
 * In order to calculate the correct depth, the actual atmospheric pressure should be measured once in air, and
 * that value should subtracted for subsequent depth calculations.
 * @param: float conversion - either Pa, bar or mbar
 * @return: float - the pressure from the sensor in the chosen format
 */
float MS5837_getPressure(float conversion)
{
	if ( _model == MS5837_02BA ) {
			return P*conversion/100.0f;
		}
		else {
			return P*conversion/10.0f;
		}
}

/**
 * @summary: returns temperature read from the module
 * @return: float - temperature in C
 */
float MS5837_getTemperature()
{
	return TEMP/100.0f;
}

/**
 * @summary: Depth returned in meters (valid for operation in incompressible
 *  liquids only. Uses density that is set for fresh or seawater.
 * @return: float - the depth in meters
 */
float MS5837_getDepth() {
	return (MS5837_getPressure(Pa)-101300)/(fluidDensity*9.80665);
}

/**
 * @summary: gets altitude (valid for operation in air only).
 * @return: float- altitude in meters
 */
float MS5837_getAltitude() {
	return (1-pow((MS5837_getPressure(1)/1013.25),.190284))*145366.45*.3048;
}

/**
 * @summary: CYCLIC REDUNDANCY CHECK (CRC). MS5837 contains a PROM memory with 112-Bit.
 * A 4-bit CRC has been implemented to check the data validity in memory.
 * @param: uint16_t n_prom[] - buffer containing 112 bit PROM memory
 * @return: uint8_t - the calculated crc
 */
uint8_t crc4(uint16_t n_prom[]) {
	uint16_t n_rem = 0;

	n_prom[0] = ((n_prom[0]) & 0x0FFF);
	n_prom[7] = 0;

	for ( uint8_t i = 0 ; i < 16; i++ ) {
		if ( i%2 == 1 ) {
			n_rem ^= (uint16_t)((n_prom[i>>1]) & 0x00FF);
		} else {
			n_rem ^= (uint16_t)(n_prom[i>>1] >> 8);
		}
		for ( uint8_t n_bit = 8 ; n_bit > 0 ; n_bit-- ) {
			if ( n_rem & 0x8000 ) {
				n_rem = (n_rem << 1) ^ 0x3000;
			} else {
				n_rem = (n_rem << 1);
			}
		}
	}

	n_rem = ((n_rem >> 12) & 0x000F);

	return n_rem ^ 0x00;
}

/**
 * @summary: One function to get the pressure and depth information
 * @param: pressure_t *pressure - pointer to pressure variable
 * @param: depth_t *depth - pointer to depth variable
 * @return: result_t - RESULT_OK if successful, RESULT_ERR if not
 */
result_t GetValues (pressure_t *pressure, depth_t *depth)
{
	result_t ret;

	ret = MS5837_read();

	if (ret != RESULT_OK)
	{
		SerialPrintln("Failed to read values from MS5837.");
		return RESULT_ERR;
	}

	*pressure = MS5837_getPressure(1.0);
	*depth = MS5837_getDepth();

	return RESULT_OK;
}
