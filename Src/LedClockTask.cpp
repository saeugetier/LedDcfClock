/*
 * LedClockTask.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "LedClockTask.h"
#include "SystemManager.h"

LedClockTask::LedClockTask(PeripheralReference<RtcClock> clock,
		PeripheralReference<WS2812<60 + 12>> leds,
		PeripheralReference<PushButton> button,
		PeripheralReference<SystemTick> tick) :
		mSystemTimerEvent(this, static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)),
		mSystemTick(tick)
{
	mMode = TaskMode::SLEEP;
	mSystemTimerEvent.getType();
	mSystemTick.init();
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
