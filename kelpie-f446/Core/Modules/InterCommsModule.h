/*
 * InterCommsModule.h
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#ifndef MODULES_INTERCOMMSMODULE_H_
#define MODULES_INTERCOMMSMODULE_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "UserTypes.h"

typedef struct ICommsMsg_t
{
	uint16_t msgId;
	uint16_t dataLen;
	void (*callback)();
} ICommsMsg_t;

typedef enum
{
	ICOMMS_OK = 0,
	ICOMMS_INVALID_MSG_LEN
}ICommsErr_t;

PUBLIC void InitInternalCommsModule();
PUBLIC result_t InternalCommsMessageCallback(PiOutgoingMessage_t msg);

#endif /* MODULES_INTERCOMMSMODULE_H_ */

