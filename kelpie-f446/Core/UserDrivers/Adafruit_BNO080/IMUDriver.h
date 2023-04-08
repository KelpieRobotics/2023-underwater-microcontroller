/*
 * IMUDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERDRIVERS_IMUDRIVER_H_
#define USERDRIVERS_IMUDRIVER_H_

#include "UserTypes.h"
#include "stm32f4xx_hal.h"
#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "FreeRTOS.h"
#include "task.h"

#define IMU_ADDRESS 0x4A
#define IMU_TIMEOUT 500 // millis

typedef struct IMUInfo_t
{
    uint16_t Reset_Pin;
    GPIO_TypeDef *Reset_Pin_Bank;
}IMUInfo_t;

PUBLIC result_t IMU_BeginI2C(uint8_t i2c_address, uint32_t sensor_ID);

PUBLIC void hardwareReset(void);
PUBLIC bool wasReset(void);

PUBLIC result_t enableReport(sh2_SensorId_t sensor);
PUBLIC bool getSensorEvent(sh2_SensorValue_t *value);

PUBLIC result_t InitIMU(int32_t sensor_id);
static uint16_t min(uint16_t a, uint16_t b); // helper function

#endif /* USERDRIVERS_IMUDRIVER_H_ */
