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
		PeripheralReference<SystemTick> tick,
		Settings& settings) :
		mSystemTimerEvent(this, static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)),
		mWakeupEvent(this, static_cast<EventType::type>(SystemEventType::PUSH_BUTTON_EVENT)),
		mSystemTick(tick),
		mLedClock(&settings)
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
