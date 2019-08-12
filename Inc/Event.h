/*
 * Event.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_EVENT_H_
#define INC_EVENT_H_

#include "stdint.h"
#include "Task.h"
#include "EventType.h"

class Event
{
public:
	Event(Task* listener, EventType::type type);
	EventType::type getType() const;
	void operator()();
protected:
	EventType::type mType;
	Task* mTask;
};

#endif /* INC_EVENT_H_ */
