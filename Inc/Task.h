/*
 * Task.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

#include "stdint.h"
#include "EventType.h"
#include "etl/list.h"

class Event;

enum class TaskMode : uint32_t
{
	WAKE,
	SLEEP,
	DEEPSLEEP,
	NONE
};

typedef etl::list<Event*, 16> EventList;

class Task
{
public:
	void setTaskMode(TaskMode mode);
	TaskMode getTaskMode() const;
	virtual void setup() = 0;
	virtual void run() = 0;
	virtual void handleEvent(EventType::type event) = 0;
	virtual EventList getEvents() = 0;
protected:
	virtual void taskModeChanged(TaskMode mode) = 0;
	TaskMode mMode = TaskMode::NONE;
};

#endif /* INC_TASK_H_ */
