/*
 * PowerSupervisorTask.h
 *
 *  Created on: 08.08.2019
 *      Author: timm
 */

#ifndef INC_POWERSUPERVISORTASK_H_
#define INC_POWERSUPERVISORTASK_H_

#include "Task.h"

class PowerSupervisorTask : public Task
{
public:
	PowerSupervisorTask();
	virtual void run();
	virtual void handleEvent(EventType::type event);
	virtual EventList getEvents();
protected:
	virtual void taskModeChanged(TaskMode mode);
};


#endif /* INC_POWERSUPERVISORTASK_H_ */
