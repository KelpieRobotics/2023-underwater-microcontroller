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

PRIVATE void HandlePiCommsMessage_t(PiCommsMessage_t msg);
PRIVATE void PrintPiCommsMessage_t(PiCommsMessage_t msg);

PUBLIC void InitInternalCommsModule(){
	PiComms_Init();
}

//calls message id's callback and passes in data
PUBLIC result_t InternalCommsMessageCallback(PiCommsMessage_t msg){
	PrintPiCommsMessage_t(msg);
	HandlePiCommsMessage_t(msg);
	return RESULT_OK;
}

PRIVATE void HandlePiCommsMessage_t(PiCommsMessage_t msg){
	if(msg.has_attachmentCommand){
		AAMod_Callback(msg.attachmentCommand);
	}
	if(msg.has_thrusterCommand){
		MCMod_ThrusterCallback(msg.thrusterCommand);
	}
}



//Helper function for  debugging. Will need to be updated when .pb.c/h files are generated just like callbacks and HandlePiCommsMessage_t
PRIVATE void PrintPiCommsMessage_t(PiCommsMessage_t msg){
	SerialPrintln("PrintPiCommsMessage_t");
	if(msg.has_attachmentCommand){
		SerialPrintln("attachmentCommand");
		SerialPrint("claw_state: %d\t", (msg.attachmentCommand.has_claw_state ? msg.attachmentCommand.claw_state : -1));
		SerialPrint("light_PWM: %d\t", (msg.attachmentCommand.has_light_PWM ? msg.attachmentCommand.light_PWM : -1));
		SerialPrint("servo_0_PWM: %d\t", (msg.attachmentCommand.has_servo_0_PWM ? msg.attachmentCommand.servo_0_PWM : -1));
		SerialPrint("servo_1_PWM: %d\t", (msg.attachmentCommand.has_servo_1_PWM ? msg.attachmentCommand.servo_1_PWM : -1));
		SerialPrint("servo_2_PWM: %d\t", (msg.attachmentCommand.has_servo_2_PWM ? msg.attachmentCommand.servo_2_PWM : -1));
		SerialPrint("result: %d\t", msg.attachmentCommand.result);
	}
	SerialPrintln("");
	if(msg.has_thrusterCommand){
		SerialPrintln("thrusterCommand");
		SerialPrint("thruster_0_PWM: %d\t", (msg.thrusterCommand.has_thruster_0_PWM ? msg.thrusterCommand.thruster_0_PWM : -1));
		SerialPrint("thruster_1_PWM: %d\t", (msg.thrusterCommand.has_thruster_1_PWM ? msg.thrusterCommand.thruster_1_PWM : -1));
		SerialPrint("thruster_2_PWM: %d\t", (msg.thrusterCommand.has_thruster_2_PWM ? msg.thrusterCommand.thruster_2_PWM : -1));
		SerialPrintln("thruster_3_PWM: %d\t", (msg.thrusterCommand.has_thruster_3_PWM ? msg.thrusterCommand.thruster_3_PWM : -1));
		SerialPrint("thruster_4_PWM: %d\t", (msg.thrusterCommand.has_thruster_4_PWM ? msg.thrusterCommand.thruster_4_PWM : -1));
		SerialPrint("thruster_5_PWM: %d\t", (msg.thrusterCommand.has_thruster_5_PWM ? msg.thrusterCommand.thruster_5_PWM : -1));
		SerialPrint("thruster_6_PWM: %d\t", (msg.thrusterCommand.has_thruster_6_PWM ? msg.thrusterCommand.thruster_6_PWM : -1));
		SerialPrintln("thruster_7_PWM: %d\t", (msg.thrusterCommand.has_thruster_7_PWM ? msg.thrusterCommand.thruster_7_PWM : -1));
		SerialPrint("result: %d\t", msg.thrusterCommand.result);
	}
	SerialPrintln("");
}
