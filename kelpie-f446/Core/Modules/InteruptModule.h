/*
 * InteruptModule.h
 *
 *  Created on: Jan 23, 2023
 *      Author: jeremycote
 */

#ifndef MODULES_INTERUPTMODULE_H_
#define MODULES_INTERUPTMODULE_H_

#include "UserTypes.h"

// Interupt IDs for each piece of data
#define NUMBER_INTERUPT_IDS 16

typedef void (*InteruptCallback)();

typedef enum InteruptLookUpIndex
{
	INTERUPT_GPIO_0_ID,
	INTERUPT_GPIO_1_ID,
	INTERUPT_GPIO_2_ID,
	INTERUPT_GPIO_3_ID,
	INTERUPT_GPIO_4_ID,
	INTERUPT_GPIO_5_ID,
	INTERUPT_GPIO_6_ID,
	INTERUPT_GPIO_7_ID,
	INTERUPT_GPIO_8_ID,
	INTERUPT_GPIO_9_ID,
	INTERUPT_GPIO_10_ID,
	INTERUPT_GPIO_11_ID,
	INTERUPT_GPIO_12_ID,
	INTERUPT_GPIO_13_ID,
	INTERUPT_GPIO_14_ID,
	INTERUPT_GPIO_15_ID,
} InteruptLookUpIndex;

typedef struct
{
	InteruptLookUpIndex index;
	InteruptCallback callback;

} InteruptInfo;

PUBLIC void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/**
 * Register a callback function with an interupt. This will override existing registered function callback.
 */
PUBLIC void InteruptRegisterCallback(InteruptLookUpIndex id, InteruptCallback callback);

PRIVATE void InteruptTriggerCallback(InteruptLookUpIndex id);

PUBLIC int8_t GetPinInteruptLookUpIndex(uint16_t pin);

#endif /* MODULES_INTERUPTMODULE_H_ */
