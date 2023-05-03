#include "picomsDataTypes_KR23.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_LENGTH 124


int main() {

	uint8_t buffer[BUFFER_LENGTH];
	memset(buffer, '\0', BUFFER_LENGTH * sizeof(uint8_t));
	size_t message_length;
	bool status;
	{
		KR23_OutgoingMessage msgOut = KR23_OutgoingMessage_init_zero;
		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
		KR23_ThrusterCommand tc = KR23_ThrusterCommand_init_zero;
		tc.thruster_0_PWM = 128;
		tc.has_thruster_0_PWM = true;
		tc.thruster_2_PWM = 0;
		tc.has_thruster_2_PWM = true;
		tc.thruster_3_PWM = 57;
		tc.has_thruster_3_PWM = true;
		tc.thruster_4_PWM = 63;
		tc.has_thruster_4_PWM = true;
		tc.thruster_7_PWM = 255;
		tc.has_thruster_7_PWM = true;
		tc.result = KR23_KelpieResult_WARNING;
		KR23_AttachmentCommand ac = KR23_AttachmentCommand_init_zero;
		ac.claw_state = KR23_KelpieClawState_OPEN;
		ac.has_claw_state = true;
		ac.servo_1_PWM = 45;
		ac.has_servo_1_PWM = true;
		ac.light_PWM = 78;
		ac.has_light_PWM = true;
		msgOut.thrusterCommand = tc;
		msgOut.has_thrusterCommand = true;
		msgOut.attachmentCommand = ac;
		msgOut.has_attachmentCommand = true;

		status = pb_encode(&stream, KR23_OutgoingMessage_fields, &msgOut);
		message_length = stream.bytes_written;
		if (!status)
		{
			printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
			return 1;
		}
	}

	printf("Buffer:\n");

	for (u_int8_t i = 0; i < message_length - 1; i++) {
		printf("%d, ", buffer[i]);
	}
	printf("%d\n", buffer[message_length - 1]);
	{
		KR23_OutgoingMessage receivedMsg = KR23_OutgoingMessage_init_zero;
		pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
		status = pb_decode(&stream, KR23_OutgoingMessage_fields, &receivedMsg);
		if (!status)
		{
			printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
			return 1;
		}
	}
	return 0;
}