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

class LedClockTask: public Task
{
public:
	LedClockTask(PeripheralReference<RtcClock> clock,
			PeripheralReference<WS2812<60 + 12>> leds,
			PeripheralReference<PushButton> button,
			PeripheralReference<SystemTick> tick);
	virtual void run();
	virtual void handleEvent(EventType::type event);
	Event& getTickEvent() { return mSystemTimerEvent; }
protected:
	virtual void taskModeChanged(TaskMode mode);
	Event mSystemTimerEvent;
	PeripheralReference<SystemTick> mSystemTick;
};

#endif /* INC_LEDCLOCKTASK_H_ */
