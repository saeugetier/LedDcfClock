/*
 * LedClockTask.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_LEDCLOCKTASK_H_
#define INC_LEDCLOCKTASK_H_

#include "Task.h"
#include "PeripheralReference.h"
#include "Clock.h"
#include "Buttons.h"
#include "WS2812.h"
#include "SystemTick.h"
#include "Event.h"
#include "LedClock.h"

class LedClockTask: public Task
{
public:
	LedClockTask(PeripheralReference<RtcClock> clock,
			PeripheralReference<WS2812<60 + 12>> leds,
			PeripheralReference<PushButton> button,
			PeripheralReference<SystemTick> tick,
			PeripheralReference<LedPowerEnable> power,
			Settings& settings);
	virtual void run();
	virtual void handleEvent(EventType::type event);
	Event& getTickEvent() { return mSystemTimerEvent; }
	Event& getWakeupEvent() { return mWakeupEvent; }
protected:
	virtual void taskModeChanged(TaskMode mode);
	bool signalizeLeds();

	Event mSystemTimerEvent;
	Event mWakeupEvent;

	PeripheralReference<RtcClock> mClock;
	PeripheralReference<WS2812<60+12>> mWS2812;
	PeripheralReference<PushButton> mPushButton;
	PeripheralReference<SystemTick> mSystemTick;
	PeripheralReference<LedPowerEnable> mLedPowerEnable;

	LedClock mLedClock;
	uint32_t mCurrentSignalizationTime;
};

#endif /* INC_LEDCLOCKTASK_H_ */
