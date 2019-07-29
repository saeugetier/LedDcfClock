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

enum class EventType : uint32_t
{
	DCF_POWER_DOWN,
	DCF_PULSE,
	DCF_WAKE_UP,
	SYSTICK_EVENT,
	DMA_BUFFER_EMPTY,
	POWER_MODE_CHANGED,
	SUPPLY_VOLTAGE_LEVEL
};

class Event
{
public:
	Event(Task* listener);
	EventType getType() const;
	void operator()();
protected:
	EventType mType;
	Task* mTask;
};

#endif /* INC_EVENT_H_ */
