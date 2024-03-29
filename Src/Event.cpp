/*
 * Event.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "Event.h"

Event::Event(Task* task,  EventType::type type) : mType(type)
{
	mTask = task;
}

void Event::operator ()()
{
	mTask->handleEvent(mType);
}

EventType::type Event::getType() const
{
	return mType;
}

