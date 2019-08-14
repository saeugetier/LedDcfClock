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
		PeripheralReference<LedPowerEnable> power,
		Settings& settings) :
		mSystemTimerEvent(this, static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)),
		mWakeupEvent(this, static_cast<EventType::type>(SystemEventType::PUSH_BUTTON_EVENT)),
		mClock(clock),
		mWS2812(leds),
		mPushButton(button),
		mSystemTick(tick),
		mLedPowerEnable(power),
		mLedClock(leds,power)
{
	setTaskMode(TaskMode::SLEEP);
}

void LedClockTask::run()
{

}

void LedClockTask::handleEvent(EventType::type event)
{
	SystemEventType::type _event = static_cast<SystemEventType::type>(event);
	switch (_event)
	{
	case SystemEventType::SYSTICK_EVENT:
		if(!signalizeLeds())
			setTaskMode(TaskMode::DEEPSLEEP);
		break;
	case SystemEventType::PUSH_BUTTON_EVENT:
		setTaskMode(TaskMode::SLEEP);
	default:
		break;
	}
}

void LedClockTask::taskModeChanged(TaskMode mode)
{
	switch (mode)
	{
	case TaskMode::SLEEP:
		mSystemTick.init();
		mClock.init();
		mPushButton.init();
		break;
	case TaskMode::DEEPSLEEP:
		mSystemTick.shutdown();
		break;
	default:
		break;
	}
}

bool LedClockTask::signalizeLeds()
{
	if(mLedClock.isReady())
	{
		if(mClock.getInstance().isClockSet())
		{
			time_t temp = mClock.getInstance().now();
			mLedClock.displayTime(*localtime(&temp), true, mClock.getInstance().getSubsecond());
		}
		else
		{
			mLedClock.displayError(LedClock::Error::TIME_NOT_SET);
		}
	}

	return false;
}
