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

class LedClockTask: public Task
{
public:
	LedClockTask(PeripheralReference<RtcClock> clock,
			PeripheralReference<WS2812<60 + 12>> leds,
			PeripheralReference<PushButton> button);
	virtual void run();
	virtual void handleEvent(EventType::type event);
protected:
	virtual void taskModeChanged(TaskMode mode);
};

#endif /* INC_LEDCLOCKTASK_H_ */
