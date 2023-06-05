/*
 * InterCommsModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy, eric
 */

//#include "PiCommsDriver.h"
#include "InterCommsModule.h"
#include "DataAggregationModule.h"
#include "PiCommsDriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UserTypes.h"
#include "MovementControlModule.h"
#include "AppendageActuationModule.h"

const char * TAG = "ICM";

PRIVATE void HandlePiCommsMessage_t(PiOutgoingMessage_t msg);
PRIVATE void PrintPiCommsMessage_t(PiOutgoingMessage_t msg);

PUBLIC void InitInternalCommsModule(){
	PiComms_Init();
}

//calls message id's callback and passes in data
PUBLIC result_t InternalCommsMessageCallback(PiOutgoingMessage_t msg){
	//PrintPiCommsMessage_t(msg);
	HandlePiCommsMessage_t(msg);
	return RESULT_OK;
}

PRIVATE void HandlePiCommsMessage_t(PiOutgoingMessage_t msg){
	if(msg.has_attachmentCommand){
		AAMod_Callback(msg.attachmentCommand);
	}
	if(msg.has_thrusterCommand){
		MCMod_ThrusterCallback(msg.thrusterCommand);
	}

	KR23_IncomingMessage imPoll = KR23_IncomingMessage_init_zero;
	imPoll.has_pollMessage = true;
	SetPollMessageValues(imPoll.pollMessage);

	PiComms_Send(imPoll);
}


//sets the poll message values
PRIVATE void SetPollMessageValues(KR23_PollMessage pollMessage){
	pollMessage.claw_state = DA_GetClawState();
	pollMessage.light_PWM = DA_GetLightValue();
	pollMessage.servo_0_PWM = DA_GetPWMServoValue(0);
	pollMessage.thruster_0_PWM = DA_GetThrusterValue(0);
	pollMessage.result = KR23_KelpieResult_OK;
}


//Helper function for  debugging. Will need to be updated when .pb.c/h files are generated just like callbacks and HandlePiCommsMessage_t
PRIVATE void PrintPiCommsMessage_t(PiOutgoingMessage_t msg){
	SerialPrintln("PrintPiCommsMessage_t");
	if(msg.has_attachmentCommand){
		SerialPrintln("attachmentCommand");
		SerialPrint("claw_state: %d\t", msg.attachmentCommand.claw_state);
		SerialPrint("light_PWM: %d\t", msg.attachmentCommand.light_PWM);
		SerialPrint("servo_0_PWM: %d\t", msg.attachmentCommand.servo_0_PWM);
		SerialPrint("servo_1_PWM: %d\t", msg.attachmentCommand.servo_1_PWM);
		SerialPrint("servo_2_PWM: %d\t", msg.attachmentCommand.servo_2_PWM);
		SerialPrint("result: %d\t", msg.attachmentCommand.result);
	}
	SerialPrintln("");
	if(msg.has_thrusterCommand){
		SerialPrintln("thrusterCommand");
		SerialPrint("thruster_0_PWM: %d\t", msg.thrusterCommand.thruster_0_PWM);
		SerialPrint("thruster_1_PWM: %d\t", msg.thrusterCommand.thruster_1_PWM);
		SerialPrint("thruster_2_PWM: %d\t", msg.thrusterCommand.thruster_2_PWM);
		SerialPrintln("thruster_3_PWM: %d\t", msg.thrusterCommand.thruster_3_PWM);
		SerialPrint("thruster_4_PWM: %d\t", msg.thrusterCommand.thruster_4_PWM);
		SerialPrint("thruster_5_PWM: %d\t", msg.thrusterCommand.thruster_5_PWM);
		SerialPrint("thruster_6_PWM: %d\t", msg.thrusterCommand.thruster_6_PWM);
		SerialPrintln("thruster_7_PWM: %d\t", msg.thrusterCommand.thruster_7_PWM);
		SerialPrint("result: %d\t", msg.thrusterCommand.result);
	}
	SerialPrintln("");
}
