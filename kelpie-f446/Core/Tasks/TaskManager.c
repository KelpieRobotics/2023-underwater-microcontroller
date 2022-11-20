/*
 * TaskManager.c
 *
 *  Created on: Oct 22, 2022
 *      Author: mingy
 */

#include "TaskManager.h"
#include "AppendageActuationTask.h"
#include "InterCommsTask.h"
#include "MovementControlTask.h"
#include "NavigationTask.h"
#include "SafetyManagerTask.h"

void RunTaskManager()
{
	InitAppendageActuationTask();
	InitInternalCommsTask();
	InitMovementControlTask();
	InitNavigationTask();
	InitSafetyTask();
}
