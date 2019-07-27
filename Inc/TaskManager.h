/*
 * TaskManager.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_TASKMANAGER_H_
#define INC_TASKMANAGER_H_

#include "Event.h"

#ifndef MAX_EVENTS
#define MAX_EVENTS 16
#endif

#ifndef MAX_TASKS
#define MAX_TASKS 8
#endif

class TaskManager
{
public:
	void invokeEvent(EventType event);
	void addTask(Task* task);
	void addEvent(Event* event);
protected:
	Event* mEventList[MAX_EVENTS];
	Task* mTaskList[MAX_TASKS];
};

#endif /* INC_TASKMANAGER_H_ */
