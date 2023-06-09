/*
 * PiCommsDriver.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef USERDRIVERS_PICOMMSDRIVER_H_
#define USERDRIVERS_PICOMMSDRIVER_H_


// STM headers
#include "stm32f4xx_hal.h"
#include "UserTypes.h"

// Standard Lib
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// Our headers
#include "UserTypes.h"

#define MAX_PI_COMMS_SEND_LENGTH 128

/* NOTE:
 * Initalization of the serial peripheral is done in main.c
 * Auto generated init by the IOC file */

PUBLIC void PiComms_Init();
PUBLIC void PiComms_Send(PiIncomingMessage_t im);

PUBLIC void PiComms_ResetByteQueue();
PUBLIC uint32_t PiComms_GetByteQueueCount();
PUBLIC uint8_t PiComms_DequeueByte();
void PiCommsUARTAbort();

#endif /* USERDRIVERS_PICOMMSDRIVER_H_ */
