/*
 * MS5837Driver.h
 *
 *  Created on: Apr 1, 2023
 *      Author: Adam
 *
 *    Adapted from: https://github.com/bluerobotics/BlueRobotics_MS5837_Library
 */

#include "UserTypes.h"
#include <stdbool.h>

static const float Pa;
static const float bar;
static const float mbar;
static const uint8_t MS5837_30BA;
static const uint8_t MS5837_02BA;
static const uint8_t MS5837_UNRECOGNISED;

void MS5837();

bool MS5837_init();
bool initialised = false;

/** Set model of MS5837 sensor. Valid options are MS5837::MS5837_30BA (default)
 * and MS5837::MS5837_02BA.
 */
void MS5837_setModel(uint8_t model);
uint8_t MS5837_getModel();

/** Provide the density of the working fluid in kg/m^3. Default is for
 * seawater. Should be 997 for freshwater.
 */
void MS5837_setFluidDensity(float density);

/** The read from I2C takes up to 40 ms, so use sparingly is possible.
 */
void MS5837_read();

/** Pressure returned in mbar or mbar*conversion rate.
 */
float MS5837_getPressure(float conversion = 1.0f);

/** Temperature returned in deg C.
 */
float MS5837_getTemperature();

/** Depth returned in meters (valid for operation in incompressible
 *  liquids only. Uses density that is set for fresh or seawater.
 */
float MS5837_getDepth();

/** Altitude returned in meters (valid for operation in air only).
 */
float MS5837_getAltitude();

uint16_t C[8];
uint32_t D1_pres, D2_temp;
int32_t TEMP;
int32_t P;
uint8_t _model;

float fluidDensity;

/** Performs calculations per the sensor data sheet for conversion and
 *  second order compensation.
 */
void calculate();

uint8_t crc4(uint16_t n_prom[]);
