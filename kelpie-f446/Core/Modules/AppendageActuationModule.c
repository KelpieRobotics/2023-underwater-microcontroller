/*
 * AppendageActuationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "AppendageActuationModule.h"
#include "DataAggregationModule.h"
#include "UserTypes.h"

#define TAG "AAM"
/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void AAMod_SetAppendageValue(claw_state_t state){
	SerialDebug(TAG, "#DEBUG: AAMod_SetAppendageValue state: %d", state);
	switch(state){
	case CLAW_OPEN:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_SET);
		break;
	case CLAW_CLOSE:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_SET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_RESET);
		break;
	default:
		DA_SetAppendageState(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageState(CLAW_R, GPIO_PIN_RESET);
	}
}

PUBLIC void AAMod_AppendageCallback(AttachmentCommand ac){
	/*
	char * state = (char *)data;
	if(state[0] == 'F') AAMod_SetAppendageValue(-1);
	else if(state[0] == '1') AAMod_SetAppendageValue(1);
	else AAMod_SetAppendageValue(0);
	*/

	/*
	 * int8_t state = (int8_t) data[0];
	 * if (state != -1 && state != 0 && state != 1) SerialDebug(TAG, "Incorrect appendage state: %d", state);
	 * AAMod_SetAppendageValue(state);
	 */
	if(ac.has_claw_state) AAMod_SetAppendageValue(ac.claw_state);

	PiComms_Send("#ACK:%s!", TAG);
}
