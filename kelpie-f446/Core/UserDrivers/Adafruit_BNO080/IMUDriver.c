/*
 * IMUDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

 // FILE MODIFIED FROM ADAFRUIT LIBRARY, FOUND HERE:
 // FIND MIT LICENSE IN LICENSE.txt

#include "IMUDriver.h"
#include "SerialDebugDriver.h"

extern I2C_HandleTypeDef hi2c2;

const IMUInfo_t imuInfo =
        {
                .Reset_Pin = GPIO_PIN_0,
                .Reset_Pin_Bank = GPIOB,
        };

static sh2_SensorValue_t *_sensor_value = NULL;
static bool _reset_occurred = false;

sh2_Hal_t _HAL; ///< The struct representing the SH2 Hardware Abstraction Layer
sh2_ProductIds_t prodIds;

static int i2chal_write(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len);
static int i2chal_read(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len,
                       uint32_t *t_us);
static void i2chal_close(sh2_Hal_t *self);
static int i2chal_open(sh2_Hal_t *self);

static uint32_t hal_getTimeUs(sh2_Hal_t *self);
static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent);
static void sensorHandler(void *cookie, sh2_SensorEvent_t *pEvent);
static void hal_hardwareReset(void);

PUBLIC result_t IMU_BeginI2C(uint8_t i2c_address, uint32_t sensor_ID)
{
    _HAL.open = i2chal_open;
    _HAL.close = i2chal_close;
    _HAL.read = i2chal_read;
    _HAL.write = i2chal_write;
    _HAL.getTimeUs = hal_getTimeUs;

    // delay to allow the sensor to power up and initialize
    vTaskDelay(100 / portTICK_PERIOD_MS);

    return InitIMU(sensor_ID);
}

PUBLIC result_t InitIMU(int32_t sensor_ID)
{
    int status;

    // Initialize the sensor
    hardwareReset();

    status = sh2_open(&_HAL, hal_callback, NULL);
    if (status != SH2_OK)
    {
        return RESULT_ERR;
    }

    // check connection by getting the chip ID
    memset(&prodIds, 0, sizeof(prodIds));
    status = sh2_getProdIds(&prodIds);

    if (status != SH2_OK) {
        return false;
    }

    // register sensor callback
    sh2_setSensorCallback(sensorHandler, NULL);

    return RESULT_OK;
}

PUBLIC void hardwareReset(void)
{
    hal_hardwareReset();
}

PUBLIC bool wasReset(void)
{
    bool reset = _reset_occurred;
    _reset_occurred = false;
    return reset;
}

PUBLIC result_t enableReport(sh2_SensorId_t sensor)
{
	uint32_t interval_us = 10000;
    static sh2_SensorConfig_t config;

    // These sensor options are disabled or not used in most cases
    config.changeSensitivityEnabled = false;
    config.wakeupEnabled = false;
    config.changeSensitivityRelative = false;
    config.alwaysOnEnabled = false;
    config.changeSensitivity = 0;
    config.batchInterval_us = 0;
    config.sensorSpecific = 0;

    config.reportInterval_us = interval_us;
    int status = sh2_setSensorConfig(sensor, &config);

    if (status != SH2_OK) {
        return RESULT_ERR;
    }

    return RESULT_OK;
}

PUBLIC bool getSensorEvent(sh2_SensorValue_t *value) {
    _sensor_value = value;

    // wait for sensor event
    value->timestamp = 0;

    sh2_service();

    if (value->timestamp == 0 && value->sensorId != SH2_GYRO_INTEGRATED_RV) {
        // no event received
        return false;
    }

    return true;
}

static int i2chal_open(sh2_Hal_t *self) {
	HAL_StatusTypeDef ret;
    // Serial.println("I2C HAL open");
    uint8_t softreset_pkt[] = {5, 0, 1, 0, 1};
    bool success = false;
    for (uint8_t attempts = 0; attempts < 5; attempts++) {
        ret = HAL_I2C_Master_Transmit(&hi2c2, IMU_ADDRESS, softreset_pkt, 5, IMU_TIMEOUT);
        if (ret == HAL_OK) {
            success = true;
            break;
        }
    }

    if (!success) return -1;

    vTaskDelay(300 / portTICK_PERIOD_MS);
    return 0;
}

static void i2chal_close(sh2_Hal_t *self) {
    // Serial.println("I2C HAL close");
}

static int i2chal_read(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len, uint32_t *t_us) {
	HAL_StatusTypeDef ret;
    // Serial.println("I2C HAL read");

    // uint8_t *pBufferOrig = pBuffer;

    uint8_t header[4];
    ret = HAL_I2C_Master_Receive(&hi2c2, IMU_ADDRESS, header, 4, IMU_TIMEOUT);
    if (ret != HAL_OK) {
        return 0;
    }

    // Determine amount to read
    uint16_t packet_size = (uint16_t)header[0] | (uint16_t)header[1] << 8;
    // Unset the "continue" bit
    packet_size &= ~0x8000;

    /*
    Serial.print("Read SHTP header. ");
    Serial.print("Packet size: ");
    Serial.print(packet_size);
    Serial.print(" & buffer size: ");
    Serial.println(len);
    */

    // STUB: set manually, look here if we have problems
    uint16_t i2c_buffer_max = 64;

    if (packet_size > len) {
        // packet wouldn't fit in our buffer
        return 0;
    }

    // the number of non-header bytes to read
    uint16_t cargo_remaining = packet_size;
    uint8_t i2c_buffer[i2c_buffer_max];
    uint16_t read_size;
    uint16_t cargo_read_amount = 0;
    bool first_read = true;

    while (cargo_remaining > 0) {
        if (first_read) {
            read_size = min(i2c_buffer_max, (size_t)cargo_remaining);
        } else {
            read_size = min(i2c_buffer_max, (size_t)cargo_remaining + 4);
        }

        // Serial.print("Reading from I2C: "); Serial.println(read_size);
        // Serial.print("Remaining to read: "); Serial.println(cargo_remaining);

        ret = HAL_I2C_Master_Receive(&hi2c2, IMU_ADDRESS, i2c_buffer, read_size, IMU_TIMEOUT);

        if (ret != HAL_OK) {
            return 0;
        }

        if (first_read) {
            // The first time we're saving the "original" header, so include it in the
            // cargo count
            cargo_read_amount = read_size;
            memcpy(pBuffer, i2c_buffer, cargo_read_amount);
            first_read = false;
        } else {
            // this is not the first read, so copy from 4 bytes after the beginning of
            // the i2c buffer to skip the header included with every new i2c read and
            // don't include the header in the amount of cargo read
            cargo_read_amount = read_size - 4;
            memcpy(pBuffer, i2c_buffer + 4, cargo_read_amount);
        }
        // advance our pointer by the amount of cargo read
        pBuffer += cargo_read_amount;
        // mark the cargo as received
        cargo_remaining -= cargo_read_amount;
    }

    /*
    for (int i=0; i<packet_size; i++) {
      Serial.print(pBufferOrig[i], HEX);
      Serial.print(", ");
      if (i % 16 == 15) Serial.println();
    }
    Serial.println();
    */

    return packet_size;
}

static int i2chal_write(sh2_Hal_t *self, uint8_t *pBuffer, unsigned len) {
	HAL_StatusTypeDef ret;
    // STUB: set manually, look here if we have problems
    uint16_t i2c_buffer_max = 64;

    /*
    Serial.print("I2C HAL write packet size: ");
    Serial.print(len);
    Serial.print(" & max buffer size: ");
    Serial.println(i2c_buffer_max);
    */

    // set a write size to the minimum of the buffer size and the length of the packet
    // to write
    uint16_t write_size = 0;

    write_size = min(i2c_buffer_max, len);

    ret = HAL_I2C_Master_Transmit(&hi2c2, IMU_ADDRESS, pBuffer, write_size, IMU_TIMEOUT);

    if (ret != HAL_OK) {
        return 0;
    }

    return write_size;
}


static void hal_hardwareReset(void) {
    if (imuInfo.Reset_Pin == -1) return;

    const TickType_t delay = 10 / portTICK_PERIOD_MS;

    // set the reset pin to high, then wait 10ms, then set it low, then wait 10ms, then set it high again
    HAL_GPIO_WritePin(imuInfo.Reset_Pin_Bank, imuInfo.Reset_Pin, GPIO_PIN_SET);
    vTaskDelay(delay);
    HAL_GPIO_WritePin(imuInfo.Reset_Pin_Bank, imuInfo.Reset_Pin, GPIO_PIN_RESET);
    vTaskDelay(delay);
    HAL_GPIO_WritePin(imuInfo.Reset_Pin_Bank, imuInfo.Reset_Pin, GPIO_PIN_SET);
    vTaskDelay(delay);
}

static uint32_t hal_getTimeUs(sh2_Hal_t *self) {
    uint32_t timeSinceBoot = xTaskGetTickCount() * portTICK_PERIOD_MS * 1000;
    // Serial.printf("I2C HAL get time: %d\n", t);
    return timeSinceBoot;
}

static void hal_callback(void *cookie, sh2_AsyncEvent_t *pEvent) {
    // If we see a reset, set a flag so that sensors will be reconfigured.
    if (pEvent->eventId == SH2_RESET) {
        // Serial.println("Reset!");
        _reset_occurred = true;
    }
}

// Handle sensor events.
static void sensorHandler(void *cookie, sh2_SensorEvent_t *event) {
    int rc;

    // Serial.println("Got an event!");

    rc = sh2_decodeSensorEvent(_sensor_value, event);
    if (rc != SH2_OK) {
        SerialPrintln("BNO08x - Error decoding sensor event");
        _sensor_value->timestamp = 0;
        return;
    }
}

static uint16_t min(uint16_t a, uint16_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}
