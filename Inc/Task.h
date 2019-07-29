/*
 * Task.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

#include "stdint.h"

enum class EventType : uint32_t;

enum class TaskMode : uint32_t
{
	WAKE,
	SLEEP,
	DEEPSLEEP,
	NONE
};

class Task
{
public:
	void setTaskMode(TaskMode mode);
	TaskMode getTaskMode() const;
	virtual void run() = 0;
	virtual void handleEvent(EventType event) = 0;
protected:
	virtual void taskModeChanged(TaskMode mode) = 0;
};

#endif /* INC_TASK_H_ */
