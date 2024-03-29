/*
 * UserTypes.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERUTILS_USERTYPES_H_
#define USERUTILS_USERTYPES_H_

#include <stdint.h>

#define PUBLIC
#define PRIVATE

typedef uint16_t length_t;
typedef float temperature_t;
typedef float humidity_t;
typedef float pressure_t;
typedef float depth_t;
typedef uint16_t angular_degrees_t;
typedef uint16_t pwm_t;
typedef uint16_t appendage_t;

typedef struct PiCommsMessage_t
{
	uint16_t messageId;
	uint16_t dataLen;
	uint8_t *data;
}PiCommsMessage_t;

typedef enum {
    RESULT_OK = 0,
    RESULT_ERR = 1,
} result_t;

#endif /* USERUTILS_USERTYPES_H_ */
