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

struct indexMap{
	uint8_t id, index;
};

PRIVATE int8_t binSearch(struct indexMap arr[], uint8_t l, uint8_t r, uint8_t msgId);
PRIVATE void swap(struct indexMap *a, struct indexMap *b);
PRIVATE int partition(struct indexMap array[], int low, int high);
PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high);

PRIVATE ICommsErr_t ParseMessageToInts(uint8_t * rawMsg, uint16_t rawMsgLen, uint8_t * intPayload); // raw messages come in ascii encoded hex, all messages are in bytes (8bits) hence each pair of chars represents a byte of data

#define NUM_MESSAGES 4

struct indexMap msgIndexMap[NUM_MESSAGES];
/* NOTE: Message length is the number of characters that make up the data payload in raw ascii encoded hex
 * 		(message with payload 020E has 2 bytes of data but message len is 4 bytes)
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |  Message Name				| Message ID (Hex)	|  Message len	|Payload breakdown	(after converting ascii to int)		|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |MCMod_ThrusterCallback		|	000A			| 4 char		| byte 0 - motor id: 0-7 								|
 * |							|					|				| byte 1 - motor pwm value: 0-255						|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |AAMod_AppendageCallback		|	000B			| 2 char		| byte 0 - appendage state: -1 open, 0 idle, 1 close	|
 * |							|					|				|														|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |AAMod_ServoCallback			|	000C			| 4 char		| byte 0 - servo id: 0-3								|
 * |							|					|				| byte 1 - servo pwm value:	0-255						|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |AAMod_SetLightValue			|	000D			| 2 char		| byte 0 - light pwm value:	0-255						|
 * |							|					|				|														|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 * |							|					|				|														|
 * |							|					|				|														|
 * +----------------------------+-------------------+---------------+-------------------------------------------------------+
 */
ICommsMsg_t msgCallbackLookup[NUM_MESSAGES] =
{
		//ID,	bytes,	callback						//UPDATE NUM_MESSAGES WHEN CALLBACKS ARE ADDED ---------------------------------------------------------- !
		{10, 	4, 		&MCMod_ThrusterCallback},
		{11,	2,		&AAMod_AppendageCallback},
		{12, 	4, 		&AAMod_ServoCallback},
		{13,	2,		&AAMod_SetLightValue},
};

// Helper function for converting a single char (hex) into int
PRIVATE uint8_t hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

// Helper function for parsing the message data payload. converts and combines char representing nybbles into integer bytes
PRIVATE ICommsErr_t ParseMessageToInts(uint8_t * rawMsg, uint16_t rawMsgLen, uint8_t * intPayload)
{
	if((rawMsgLen & 0x01) == 1)
	{
		return ICOMMS_INVALID_MSG_LEN;
	}
	for(int i = 0; i<rawMsgLen; i+=2)
	{
		intPayload[i/2] = (hex2int(rawMsg[i]) << 4) | hex2int(rawMsg[i+1]);
		SerialDebug(TAG, "payload %d: %d", i/2, intPayload[i/2]);
	}
	return ICOMMS_OK;
}

//setup mapping message ids to msgCallbackLookup indexes
PRIVATE void MakeInternalCommsMapping(){
	for(uint8_t i = 0; i < NUM_MESSAGES; i++){
		msgIndexMap[i].id = msgCallbackLookup[i].msgId;
		msgIndexMap[i].index = i;
	}
}

//binary search for message by id
PRIVATE int8_t binSearch(struct indexMap arr[], uint8_t l, uint8_t r, uint8_t msgId)
{
    if (r >= l) {
        int8_t mid = l + ((r - l)>>1);
        if (arr[mid].id == msgId) return arr[mid].index;
        if (arr[mid].id > msgId) return binSearch(arr, l, mid - 1, msgId);
        return binSearch(arr, mid + 1, r, msgId);
    }
    return -1;
}

PUBLIC void InitInternalCommsModule(){
	PiComms_Init();
	MakeInternalCommsMapping();
	SortICommsMsg(msgIndexMap, 0, NUM_MESSAGES-1);
	SerialDebug(TAG, " Init completed ");
}

//calls message id's callback and passes in data
PUBLIC result_t InternalCommsMessageCallback(PiCommsMessage_t msg){
	int8_t msgIndex = binSearch(msgIndexMap, 0, NUM_MESSAGES-1, msg.messageId);
	if(msgIndex < 0){
		SerialPrintln("#ERR: InternalComms msgId %d not found", msg.messageId);
		PiComms_Send("#ERR:",TAG,",NoMsgId!");
		return RESULT_ERR;
	}
	if(msgCallbackLookup[msgIndex].dataLen != msg.dataLen){
		SerialPrintln("#ERR: InternalCommsMessageCallback message dataLen incorrect length. Given %d, Expected %d, msgIndex: %d", msg.dataLen, msgCallbackLookup[msgIndex].dataLen, msgIndex);
		PiComms_Send("#ERR:",TAG,",WrongMsgLen!");
		return RESULT_ERR;
	}
	uint8_t intPayload[128]; // ascii encoded hex converted int8
	ICommsErr_t res = ParseMessageToInts(msg.data, msg.dataLen, intPayload); // checks for even number of chars in ascii encoded hex data payload

	SerialDebug(TAG, "Parse Result: %d", res);
	msgCallbackLookup[msgIndex].callback(intPayload);
	return RESULT_OK;
}



//////////////////////////////////////////////////////////////////////////////////////////////

//FOR SORTING SortICommsMsg

//////////////////////////////////////////////////////////////////////////////////////////////

PRIVATE void swap(struct indexMap *a, struct indexMap *b) {
	struct indexMap t = *a;
  *a = *b;
  *b = t;
}

PRIVATE int partition(struct indexMap array[], int low, int high) {
  int pivot = array[high].id;
  int i = (low - 1);

  for (int j = low; j < high; j++) {
    if (array[j].id <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i + 1], &array[high]);
  return (i + 1);
}

PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high) {
  if (low < high) {
    int pi = partition(array, low, high);
    SortICommsMsg(array, low, pi - 1);
    SortICommsMsg(array, pi + 1, high);
  }
}

