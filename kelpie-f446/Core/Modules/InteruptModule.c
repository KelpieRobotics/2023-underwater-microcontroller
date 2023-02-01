/*
 * InteruptModule.h
 *
 *  Created on: Jan 31, 2023
 *      Author: jeremycote
 */
 
 #include "InteruptModule.h"
#include "SerialDebugDriver.h"


#define DebugPrint(...) SerialPrintln(__VA_ARGS__)

const char ITM_TAG[] = "#ITM:";

/*********************************************************************************
 *
 * 		Look up table for Interupts and their callbacks
 *
 **********************************************************************************/

static InteruptInfo interuptLookUpTable[NUMBER_INTERUPT_IDS] =
	{
			// Message Index			Callback
			{INTERUPT_GPIO_0_ID,			NULL},
			{INTERUPT_GPIO_1_ID,			NULL},
			{INTERUPT_GPIO_2_ID,			NULL},
			{INTERUPT_GPIO_3_ID,			NULL},
			{INTERUPT_GPIO_4_ID,			NULL},
			{INTERUPT_GPIO_5_ID,			NULL},
			{INTERUPT_GPIO_6_ID,			NULL},
			{INTERUPT_GPIO_7_ID,			NULL},
			{INTERUPT_GPIO_8_ID,			NULL},
			{INTERUPT_GPIO_9_ID,			NULL},
			{INTERUPT_GPIO_10_ID,			NULL},
			{INTERUPT_GPIO_11_ID,			NULL},
			{INTERUPT_GPIO_12_ID,			NULL},
			{INTERUPT_GPIO_13_ID,			NULL},
			{INTERUPT_GPIO_14_ID,			NULL},
			{INTERUPT_GPIO_15_ID,			NULL}
	};

PUBLIC void InteruptRegisterCallback(InteruptLookUpIndex id, InteruptCallback callback)
{
	//SerialPrintln("A InteruptRegisterCallback: %p: ", callback);
	//callback();
	interuptLookUpTable[id].callback = callback;
	//SerialPrintln("B InteruptRegisterCallback: %p: ", interuptLookUpTable[id].callback);
	//interuptLookUpTable[id].callback();
}


PRIVATE void InteruptTriggerCallback(InteruptLookUpIndex id) {

	if (interuptLookUpTable[id].callback == NULL) {
		DebugPrint("%s No interupt register for GPIO [%d]", ITM_TAG, id);
		return;
	}

	interuptLookUpTable[id].callback();
}

/*********************************************************************************
 *
 * 		STM32 Interupt Callback
 *
 **********************************************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
		case GPIO_PIN_0:
			InteruptTriggerCallback(INTERUPT_GPIO_0_ID);
			return;
		case GPIO_PIN_1:
			InteruptTriggerCallback(INTERUPT_GPIO_1_ID);
			return;
		case GPIO_PIN_2:
			InteruptTriggerCallback(INTERUPT_GPIO_2_ID);
			return;
		case GPIO_PIN_3:
			InteruptTriggerCallback(INTERUPT_GPIO_3_ID);
			return;
		case GPIO_PIN_4:
			InteruptTriggerCallback(INTERUPT_GPIO_4_ID);
			return;
		case GPIO_PIN_5:
			InteruptTriggerCallback(INTERUPT_GPIO_5_ID);
			return;
		case GPIO_PIN_6:
			InteruptTriggerCallback(INTERUPT_GPIO_6_ID);
			return;
		case GPIO_PIN_7:
			InteruptTriggerCallback(INTERUPT_GPIO_7_ID);
			return;
		case GPIO_PIN_8:
			InteruptTriggerCallback(INTERUPT_GPIO_8_ID);
			return;
		case GPIO_PIN_9:
			InteruptTriggerCallback(INTERUPT_GPIO_9_ID);
			return;
		case GPIO_PIN_10:
			InteruptTriggerCallback(INTERUPT_GPIO_10_ID);
			return;
		case GPIO_PIN_11:
			InteruptTriggerCallback(INTERUPT_GPIO_11_ID);
			return;
		case GPIO_PIN_12:
			InteruptTriggerCallback(INTERUPT_GPIO_12_ID);
			return;
		case GPIO_PIN_13:
			InteruptTriggerCallback(INTERUPT_GPIO_13_ID);
			return;
		case GPIO_PIN_14:
			InteruptTriggerCallback(INTERUPT_GPIO_14_ID);
			return;
		case GPIO_PIN_15:
			InteruptTriggerCallback(INTERUPT_GPIO_15_ID);
			return;
		default:
			DebugPrint("%s GPIO Pin [%d] Invalid Interupt.", ITM_TAG, GPIO_Pin);
			return;
	}
}