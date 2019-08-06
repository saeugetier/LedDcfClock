/*
 * LedClockTask.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_LEDCLOCKTASK_H_
#define INC_LEDCLOCKTASK_H_

#include "Task.h"

class LedClockTask : public Task
{
public:
	virtual void run();
	virtual void handleEvent(EventType::type event);
protected:
	virtual void taskModeChanged(TaskMode mode);
};

#endif /* INC_LEDCLOCKTASK_H_ */
