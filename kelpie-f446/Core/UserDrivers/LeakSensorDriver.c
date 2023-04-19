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
#include "SafetyManagerModule.h"

#define TAG "LSD"


PRIVATE void LeakSensorIntCallback(){
	SerialDebug(TAG, "LeakDetected");
	SMModLeakSensorERR();
}

/**
 * @brief: Initializes the leak sensor interrupt.
 * @return: Result of the initiation.
 */
PUBLIC result_t InitLeakSensor(){
	int8_t pinId = GetPinInteruptLookUpIndex(LEAK_EXTI_Pin);
	if(pinId < 0) return RESULT_ERR;
	InteruptRegisterCallback(pinId, LeakSensorIntCallback);
	return RESULT_OK;
}
