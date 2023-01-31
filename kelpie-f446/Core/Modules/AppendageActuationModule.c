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
	switch(state){
	case CLAW_OPEN:
		DA_SetAppendageValue(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageValue(CLAW_R, GPIO_PIN_SET);
		break;
	case CLAW_CLOSE:
		DA_SetAppendageValue(CLAW_L, GPIO_PIN_SET);
		DA_SetAppendageValue(CLAW_R, GPIO_PIN_RESET);
		break;
	default:
		DA_SetAppendageValue(CLAW_L, GPIO_PIN_RESET);
		DA_SetAppendageValue(CLAW_R, GPIO_PIN_RESET);
	}
}
