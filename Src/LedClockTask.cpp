/*
 * LedClockTask.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "LedClockTask.h"

LedClockTask::LedClockTask(PeripheralReference<RtcClock> clock,
		PeripheralReference<WS2812<60 + 12>> leds,
		PeripheralReference<PushButton> button)
{
	mMode = TaskMode::SLEEP;
}

void LedClockTask::run()
{

}

void LedClockTask::handleEvent(EventType::type event)
{

}

void LedClockTask::taskModeChanged(TaskMode mode)
{

}
