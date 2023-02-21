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



void TestCallback1();
void TestCallback2();
void TestCallback3();
void TestCallback4();

struct indexMap{
	uint8_t id, index;
};

PRIVATE int8_t binSearch(struct indexMap arr[], uint8_t l, uint8_t r, uint8_t msgId);
PRIVATE void swap(struct indexMap *a, struct indexMap *b);
PRIVATE int partition(struct indexMap array[], int low, int high);
PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high);

struct indexMap *msgIndexMap;

#define NUM_MESSAGES 4

ICommsMsg_t msgCallbackLookup[NUM_MESSAGES] =
{
		// ID
		{4, 8, &TestCallback1},
		{6, 8, &TestCallback2},
		{33, 8, &TestCallback3},
		{1, 8, &TestCallback4}
};

PRIVATE void MakeInternalCommsMapping(){
	msgIndexMap = malloc(sizeof(struct indexMap) * NUM_MESSAGES);
	for(uint8_t i = 0; i < NUM_MESSAGES; i++){
		msgIndexMap[i].id = msgCallbackLookup[i].msgId;
		msgIndexMap[i].index = i;
	}
}

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

PUBLIC result_t InternalCommsMessageCallback(uint8_t msgId, uint8_t dataLen, void *data){
	int8_t msgIndex = binSearch(msgIndexMap, 0, NUM_MESSAGES-1, msgId);
	if(msgIndex < 0){
		SerialPrintln("InternalComms msgId %d not found", msgId);
		return RESULT_ERR;
	}
	if(msgCallbackLookup[msgIndex].dataLen != dataLen){
		SerialPrintln("InternalComms dataLen incorrect length. Given %d, Expected %d", dataLen, msgCallbackLookup[msgIndex].dataLen);
		return RESULT_ERR;
	}

	msgCallbackLookup[msgIndex].callback(data);
	return RESULT_OK;
}

void TestCallback1(void *data){
	SerialPrintln("TestCallback1");
}
void TestCallback2(void *data){
	SerialPrintln("TestCallback2");
}
void TestCallback3(void *data){
	SerialPrintln("TestCallback3");
}
void TestCallback4(void *data){
	SerialPrintln("TestCallback4");
}


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

