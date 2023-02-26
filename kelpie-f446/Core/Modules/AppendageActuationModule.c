/*
 * AppendageActuationModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

#include "AppendageActuationModule.h"
#include "DataAggregationModule.h"
#include "UserTypes.h"


/*
 * converts the input to thruster , then calls DA_SetThrusterValue
 */
PUBLIC void AAMod_SetAppendageValue(claw_state_t state){
	SerialPrintln("#DEBUG: AAMod_SetAppendageValue state: %d", state);
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

PUBLIC void AAMod_AppendageCallback(void * data){
	char * state = (char *)data;
	if(state[0] == 'F') AAMod_SetAppendageValue(-1);
	else if(state[0] == '1') AAMod_SetAppendageValue(1);
	else AAMod_SetAppendageValue(0);
}
