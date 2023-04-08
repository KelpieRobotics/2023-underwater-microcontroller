//
// Created by Juan Hiedra Primera on 11/29/2022.
//

#include "SHT30DDriver.h"

// replace hi2c1 with a different handle if required
extern I2C_HandleTypeDef hi2c1;

/**
 * @summary: Gets values from I2C sensor
 * @param: temperature_t *temperature - pointer to temperature value
 * @param: humidity_t *humidity - pointer to humidity value
 * @return: SHT30D_Status_t - status of the operation
 */
PUBLIC SHT30D_Status_t SHT30D_GetValues(temperature_t *temperature, humidity_t *humidity)
{
    HAL_StatusTypeDef ret;
    uint8_t buffer[6];
    uint16_t rawI2CValue = 0;

    buffer[0] = REGISTER_HIGH_REPEATABILITY_MEASUREMENT_MSB;
    buffer[1] = REGISTER_HIGH_REPEATABILITY_MEASUREMENT_LSB;

    //// write 2 bytes as a command to the buffer
    ret = HAL_I2C_Master_Transmit(&hi2c1, SENSOR_ADDRESS, buffer, 2, SENSOR_TIMEOUT);

    //// if the write didn't return the OK flag, abort current read
    if (ret != HAL_OK)
    {
        return SHT30D_ERROR_WRITE;
    }

    //// read 6 bytes from the sensor address and combine them into the raw value
    //// bytes are [TMP_MSB, TMP_LSB, CRC, HUM_MSB, HUM_LSB, CRC] in order
    ret = HAL_I2C_Master_Receive(&hi2c1, SENSOR_ADDRESS, buffer, 6, SENSOR_TIMEOUT);

    //// if the read didn't return the OK flag, abort calculations
    if (ret != HAL_OK)
    {
        return SHT30D_ERROR_READ;
    }

    //// combine values into temperature and humidity (can be separate function)
    rawI2CValue |= buffer[0] << 8;
    rawI2CValue |= buffer[1];

    *temperature = -45.0 + (175.0 * (float)((float)rawI2CValue / (float)((2<<15) - 1)));

    rawI2CValue |= buffer[3] << 8;
    rawI2CValue |= buffer[4];

    *humidity = (100 * (float)rawI2CValue) / (float)((2<<15) - 1);

    return SHT30D_OK;
}
