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
		mUndervoltageEvent(this, static_cast<EventType::type>(SystemEventType::UNDERVOLTAGE_SHUTDOWN)),
		mClock(clock),
		mWS2812(leds),
		mPushButton(button),
		mSystemTick(tick),
		mLedPowerEnable(power),
		mSettings(settings),
		mLedClock(leds,power),
		mCurrentSignalizationTimeout(0),
		mCurrentSignalizationType(SignalizationType::NONE)
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
		{
			stopSignalization();
			setTaskMode(TaskMode::DEEPSLEEP);
		}
		break;
	case SystemEventType::CLOCK_ALARM_EVENT:
		setTaskMode(TaskMode::SLEEP);
		startSignalization(2000);
		break;
	case SystemEventType::PUSH_BUTTON_EVENT:
		setTaskMode(TaskMode::SLEEP);
		startSignalization(8000);
		break;
	case SystemEventType::UNDERVOLTAGE_SHUTDOWN:
		stopSignalization();
		setTaskMode(TaskMode::DEEPSLEEP);
		break;
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
	if(mCurrentSignalizationType == SignalizationType::NONE)
		return false;


	if(mLedClock.isReady() && !(mCurrentSignalizationTimeout % 20)) //every 20ms
	{
		mLedClock.setBrightness(mSettings.getBrightness());

		if(mCurrentSignalizationType == SignalizationType::TIME)
		{
			time_t temp = mClock.getInstance().now();
			mLedClock.displayTime(*localtime(&temp), true, mClock.getInstance().getSubsecond());
		}
		else if(mCurrentSignalizationType == SignalizationType::ERROR_NOT_SET)
		{
			mLedClock.displayError(LedClock::Error::TIME_NOT_SET);
		}
		else if(mCurrentSignalizationType == SignalizationType::ERROR_HARDWARE)
		{
			mLedClock.displayError(LedClock::Error::HARDWARE_FAULT);
		}
	}

	if(mCurrentSignalizationTimeout > 0)
	{
		mCurrentSignalizationTimeout--;
		return true;
	}

	return false;
}

void LedClockTask::startSignalization(uint32_t length)
{
	if(mClock.getInstance().isClockSet())
	{
		mCurrentSignalizationType = SignalizationType::TIME;
		mCurrentSignalizationTimeout = length;
	}
	else
	{
		mCurrentSignalizationType = SignalizationType::ERROR_NOT_SET;
		mCurrentSignalizationTimeout = 5000; //error is shown 5 seconds
	}
}

void LedClockTask::stopSignalization()
{
	mCurrentSignalizationType = SignalizationType::NONE;
}

EventList LedClockTask::getEvents()
{
	EventList list;
	list.push_front(&mSystemTimerEvent);
	list.push_front(&mWakeupEvent);
	list.push_front(&mClockEvent);
	return list;
}
