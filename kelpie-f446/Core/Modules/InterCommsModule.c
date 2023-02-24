/*
 * InterCommsModule.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

//#include "PiCommsDriver.h"
#include "InterCommsModule.h"
#include "DataAggregationModule.h"
#include "PiCommsDriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UserTypes.h"

/*
check message queue
check message ID
do binary search for msgID in Data struct in module layer (see below)
call callback
//may want helper function that dbl checks dataLen (report error to serial terminal (debug print))

Data struct in module layer: msgID, dataLen, callbackForHowItShouldBeHandled
*/


//NOT FINAL: USED FOR TESTING
void TestCallback1();
void TestCallback2();
void TestCallback3();

struct indexMap{
	uint8_t id, index;
};

PRIVATE int8_t binSearch(struct indexMap arr[], uint8_t l, uint8_t r, uint8_t msgId);
PRIVATE void swap(struct indexMap *a, struct indexMap *b);
PRIVATE int partition(struct indexMap array[], int low, int high);
PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high);


#define NUM_MESSAGES 4

struct indexMap msgIndexMap[NUM_MESSAGES];
ICommsMsg_t msgCallbackLookup[NUM_MESSAGES] =
{
		//ID,	bytes,	callback
		{4, 	2, 		&TestCallback1},
		{6, 	4, 		&TestCallback2},
		{33, 	3, 		&TestCallback3},
};

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
}

//calls message id's callback and passes in data
PUBLIC result_t InternalCommsMessageCallback(PiCommsMessage_t msg){
	int8_t msgIndex = binSearch(msgIndexMap, 0, NUM_MESSAGES-1, msg.messageId);
	if(msgIndex < 0){
		SerialPrintln("InternalComms msgId %d not found", msg.messageId);
		return RESULT_ERR;
	}
	if(msgCallbackLookup[msgIndex].dataLen != msg.dataLen){
		SerialPrintln("InternalComms message dataLen incorrect length. Given %d, Expected %d", msg.dataLen, msgCallbackLookup[msgIndex].dataLen);
		return RESULT_ERR;
	}

	msgCallbackLookup[msgIndex].callback(msg.data);
	return RESULT_OK;
}

// data length = 2 * sizeof(char)
void TestCallback1(void *data){
	char * localData = (char*)data;
	SerialPrintln("TestCallback1: %c%c",localData[0],localData[1]);
}

// data length = 2 * sizeof(uint16_t) = 4 * sizeof(char)
void TestCallback2(void *data){
	uint16_t * localData = (uint16_t*)data;
	SerialPrintln("TestCallback2: %d%d",localData[0],localData[1]);
}

// data length = 3 * sizeof(uint8_t)
void TestCallback3(void *data){
	uint8_t * localData = (uint8_t*)data;
	SerialPrintln("TestCallback3: %d%d%d",localData[0],localData[1],localData[2]);
}



//FOR SORT SortICommsMsg
PRIVATE void swap(struct indexMap *a, struct indexMap *b) {
	struct indexMap t = *a;
  *a = *b;
  *b = t;
}
//FOR SORT SortICommsMsg
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
//FOR SORT SortICommsMsg
PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high) {
  if (low < high) {
    int pi = partition(array, low, high);
    SortICommsMsg(array, low, pi - 1);
    SortICommsMsg(array, pi + 1, high);
  }
}

