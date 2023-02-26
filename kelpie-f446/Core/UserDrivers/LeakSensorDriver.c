/*
 * LeakSensorDriver.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "LeakSensorDriver.h"
#include "SerialDebugDriver.h"
 #include "InteruptModule.h"
 #include "main.h"


PRIVATE void LeakSensorIntCallback(){
	SerialPrintln("\tLeakSensorIntCallback");

}

PUBLIC result_t InitLeakSensor(){
	int8_t pinId = GetPinInteruptLookUpIndex(LEAK_EXTI_Pin);
	if(pinId < 0) return RESULT_ERR;
	InteruptRegisterCallback(pinId, LeakSensorIntCallback);
	return RESULT_OK;
}
