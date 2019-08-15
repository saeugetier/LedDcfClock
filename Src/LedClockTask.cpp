/*
 * LedClockTask.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "LedClockTask.h"
#include "SystemManager.h"
#include "Settings.h"

LedClockTask::LedClockTask(PeripheralReference<RtcClock> clock,
		PeripheralReference<WS2812<60 + 12>> leds,
		PeripheralReference<PushButton> button,
		PeripheralReference<SystemTick> tick,
		PeripheralReference<LedPowerEnable> power,
		Settings& settings) :
		mSystemTimerEvent(this, static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)),
		mWakeupEvent(this, static_cast<EventType::type>(SystemEventType::PUSH_BUTTON_EVENT)),
		mClockEvent(this, static_cast<EventType::type>(SystemEventType::CLOCK_ALARM_EVENT)),
		mClock(clock),
		mWS2812(leds),
		mPushButton(button),
		mSystemTick(tick),
		mLedPowerEnable(power),
		mSettings(settings),
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
		startSignalization();
	default:
		break;
	}
}

void LedClockTask::taskModeChanged(TaskMode mode)
{
	mPushButton.init();  //push button is allways initialized -> wakeup source

	switch (mode)
	{
	case TaskMode::SLEEP:
		mSystemTick.init();
		mClock.init();
		mLedPowerEnable.init();
		mWS2812.init();
		mLedClock.setPower(true);
		break;
	case TaskMode::DEEPSLEEP:
		mLedClock.setPower(false);
		mSystemTick.shutdown();
		mLedPowerEnable.shutdown();
		mClock.shutdown();
		mWS2812.shutdown();
		break;
	default:
		break;
	}
}

bool LedClockTask::signalizeLeds()
{
	if(mLedClock.isReady())
	{
		mLedClock.setBrightness(mSettings.getBrightness());

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

	if(mCurrentSignalizationTime < 100)
	{
		mCurrentSignalizationTime++;
		return true;
	}

	return false;
}

void LedClockTask::startSignalization()
{
	mCurrentSignalizationTime = 0;
}

EventList LedClockTask::getEvents()
{
	EventList list;
	list.push_front(&mSystemTimerEvent);
	list.push_front(&mWakeupEvent);
	list.push_front(&mClockEvent);
	return list;
}
