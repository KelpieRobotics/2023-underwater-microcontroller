/*
 * LeakSensorDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "LeakSensorDriver.h"
#include "SerialDebugDriver.h"
 #include "InteruptModule.h"

#define PIN_NUM 13
const uint16_t GPIO_Pin = GPIO_PIN_13;


PRIVATE void LeakSensorIntCallback(){
	SerialPrintln("\tLeakSensorIntCallback");

}

PUBLIC result_t InitLeakSensor(){
//	InteruptRegisterCallback();
//	if(HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) == GPIO_PIN_RESET){
//		LeakSensorIntCallback();
//		return RESULT_ERR;
//	}
//	return RESULT_OK;

	InteruptRegisterCallback(PIN_NUM, LeakSensorIntCallback);
	return RESULT_OK;
}
