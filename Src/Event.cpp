/*
 * Event.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "Event.h"

Event::Event(Task* task)
{
	mTask = task;
}

void Event::operator ()()
{
	mTask->handleEvent(mType);
}

EventType Event::getType() const
{
	return mType;
}

