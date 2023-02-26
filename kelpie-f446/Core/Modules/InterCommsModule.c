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

struct indexMap{
	uint8_t id, index;
};

PRIVATE int8_t binSearch(struct indexMap arr[], uint8_t l, uint8_t r, uint8_t msgId);
PRIVATE void swap(struct indexMap *a, struct indexMap *b);
PRIVATE int partition(struct indexMap array[], int low, int high);
PRIVATE void SortICommsMsg(struct indexMap array[], int low, int high);

#define NUM_MESSAGES 2

struct indexMap msgIndexMap[NUM_MESSAGES];
ICommsMsg_t msgCallbackLookup[NUM_MESSAGES] =
{
		//ID,	bytes,	callback				//UPDATE NUM_MESSAGES WHEN CALLBACKS ARE ADDED
		{42, 	4, 		&MCMod_ThrusterCallback},
		{15,	1,		&AAMod_AppendageCallback},
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
		SerialPrintln("#ERR: InternalComms msgId %d not found", msg.messageId);
		return RESULT_ERR;
	}
	if(msgCallbackLookup[msgIndex].dataLen != msg.dataLen){
		SerialPrintln("#ERR: InternalCommsMessageCallback message dataLen incorrect length. Given %d, Expected %d, msgIndex: %d", msg.dataLen, msgCallbackLookup[msgIndex].dataLen, msgIndex);
		return RESULT_ERR;
	}

	msgCallbackLookup[msgIndex].callback(msg.data);
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

