/*
 * TaskManager.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_TASKMANAGER_H_
#define INC_TASKMANAGER_H_

#include "Event.h"
#include "Callback.h"

#ifndef MAX_EVENTS
#define MAX_EVENTS 16
#endif

#ifndef MAX_TASKS
#define MAX_TASKS 8
#endif

class TaskManager
{
public:
	TaskManager();
	void invokeEvent(EventType event);
	void addTask(Task* task);
	void addEvent(Event* event);
	void runTasks();
protected:
	Event* mEventList[MAX_EVENTS];
	Task* mTaskList[MAX_TASKS];
	void sleep();
	void deepSleep();
};

template<EventType type>
class TaskCallback: public Callback
{
protected:
	TaskManager& mManager;
public:
	TaskCallback(TaskManager& manager) :
			mManager(manager)
	{
	}
	void notify()
	{
		mManager.invokeEvent(type);
	}
};

#endif /* INC_TASKMANAGER_H_ */
