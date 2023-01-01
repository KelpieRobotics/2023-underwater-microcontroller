/*
 * SHT30DDriver.h
 *
 *  Created on: Nov 29, 2022
 *      Author: Juan Hiedra
 */

#ifndef SRC_SHT30DDRIVER_H_
#define SRC_SHT30DDRIVER_H_

// STM headers
#include "stm32f4xx_hal.h"

// Standard Lib
#include <stdint.h>

// Our APIs
#include "UserTypes.h"

#define REGISTER_HIGH_REPEATABILITY_MEASUREMENT_MSB 0x2C
#define REGISTER_HIGH_REPEATABILITY_MEASUREMENT_LSB 0x06
#define SENSOR_ADDRESS (0x44 << 1)
#define SENSOR_TIMEOUT 500 // millis

/*
 * @summary:
 */
typedef enum {
    SHT30D_OK = 0,
    SHT30D_ERROR_WRITE = 1,
    SHT30D_ERROR_READ = 2
} SHT30D_Status_t;

/*
 * NOTE:
 * Initalization of the I2C peripheral is done in main.c
 * Auto generated init by the IOC file
 */

PUBLIC SHT30D_Status_t SHT30D_GetValues(temperature_t *temperature, humidity_t *humidity);

#endif /* SRC_SHT30DDRIVER_H_ */
