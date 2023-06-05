#include "picomsDataTypes_KR23_proto3.pb.h"
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
		uint8_t MAX = 127;
		KR23_IncomingMessage msgIn = KR23_IncomingMessage_init_zero;
		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
		KR23_PollMessage pm = KR23_PollMessage_init_zero;
		pm.thruster_0_PWM = MAX;
		pm.thruster_1_PWM = MAX;
		pm.thruster_2_PWM = MAX;
		pm.thruster_3_PWM = MAX;
		pm.thruster_4_PWM = MAX;
		pm.thruster_5_PWM = MAX;
		pm.thruster_6_PWM = MAX;
		pm.thruster_7_PWM = MAX;
		pm.claw_state = KR23_KelpieClawState_OPEN;
		pm.light_PWM = MAX;
		pm.servo_0_PWM = MAX;
		pm.servo_1_PWM = MAX;
		pm.servo_2_PWM = MAX;
		pm.result = KR23_KelpieResult_OK;
		msgIn.pollMessage = pm;
		msgIn.has_pollMessage = true;

		status = pb_encode(&stream, KR23_IncomingMessage_fields, &msgIn);
		message_length = stream.bytes_written;
		if (!status)
		{
			printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
			return 1;
		}
	}

	printf("Buffer: %ld\n", message_length);

	for (u_int8_t i = 0; i < message_length - 1; i++) {
		printf("%d ", buffer[i]);
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