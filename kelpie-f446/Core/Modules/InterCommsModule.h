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

typedef struct ICommsMsg_t{
	uint8_t msgId;
	uint8_t dataLen;
	void (*callback)();
} ICommsMsg_t;

PUBLIC void InitInternalCommsModule();
PUBLIC result_t InternalCommsMessageCallback(uint8_t msgId, uint8_t dataLen, void *data);

#endif /* MODULES_INTERCOMMSMODULE_H_ */

