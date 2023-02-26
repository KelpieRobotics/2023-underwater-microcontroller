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

uint16_t GPIO_PIN_ARRAY[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

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
	interuptLookUpTable[id].callback = callback;
}


PRIVATE void InteruptTriggerCallback(InteruptLookUpIndex id) {

	if (interuptLookUpTable[id].callback == NULL) {
		DebugPrint("%s No interupt register for GPIO [%d]", ITM_TAG, id);
		return;
	}

	interuptLookUpTable[id].callback();
}

PRIVATE int8_t binSearchPinInterupt(uint16_t arr[], int8_t l, int8_t r, uint16_t pin)
{
	if (r >= l) {
		int8_t mid = l + ((r - l)>>1);
		if (arr[mid] == pin) return mid;
		if (arr[mid] > pin) return binSearchPinInterupt(arr, l, mid - 1, pin);
		return binSearchPinInterupt(arr, mid + 1, r, pin);
	}
	return -1;
}

PUBLIC int8_t GetPinInteruptLookUpIndex(uint16_t pin){
	return binSearchPinInterupt(GPIO_PIN_ARRAY, 0, NUMBER_INTERUPT_IDS, pin);
}

/*********************************************************************************
 *
 * 		STM32 Interupt Callback
 *
 **********************************************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	int8_t pinId = GetPinInteruptLookUpIndex(GPIO_Pin);
	if(pinId < 0){
		DebugPrint("%s GPIO Pin [%d] Invalid Interupt.", ITM_TAG, GPIO_Pin);
		return;
	}
	InteruptTriggerCallback(pinId);
}
