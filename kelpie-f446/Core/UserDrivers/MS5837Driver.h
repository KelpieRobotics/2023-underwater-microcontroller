/*
 * MS5837Driver.h
 *
 *  Created on: Apr 1, 2023
 *      Author: Adam
 *
 *    Adapted from: https://github.com/bluerobotics/BlueRobotics_MS5837_Library
 *
 *    The MIT License (MIT)
 *  Copyright (c) 2015 Blue Robotics Inc.
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#ifndef SRC_MS5837DRIVER_H_
#define SRC_MS5837DRIVER_H_

#include "UserTypes.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

void MS5837();

PUBLIC result_t MS5837_init();

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
PUBLIC result_t MS5837_read();

/** Pressure returned in mbar or mbar*conversion rate.
 */
float MS5837_getPressure(float conversion);

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

PUBLIC result_t MS5837_getValues(pressure_t *pressure, depth_t *depth);

/** Performs calculations per the sensor data sheet for conversion and
 *  second order compensation.
 */
void MS5837_calculate();

uint8_t crc4(uint16_t n_prom[]);
#endif
