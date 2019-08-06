/*
 * Task.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "Task.h"

void Task::setTaskMode(TaskMode mode)
{
	if(mode != mMode)
	{
		mMode = mode;
		taskModeChanged(mode);
	}
}

TaskMode Task::getTaskMode() const
{
	return mMode;
}
