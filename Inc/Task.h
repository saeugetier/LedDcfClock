/*
 * Task.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

enum class TaskMode : uint32_t
{
	WAKE,
	SLEEP,
	DEEPSLEEP
};

class Task
{
public:
	void setTaskMode(TaskMode mode);
	TaskMode getTaskMode() const;
protected:
	virtual void taskModeChanged(TaskMode mode) = 0;
};

#endif /* INC_TASK_H_ */
