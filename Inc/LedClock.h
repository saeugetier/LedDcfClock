/*
 * LedClock.h
 *
 *  Created on: 04.07.2019
 *      Author: timm
 */

#ifndef INC_LEDCLOCK_H_
#define INC_LEDCLOCK_H_

#include "PeripheralReference.h"
#include "WS2812.h"
#include "LedPowerEnable.h"
#include "Settings.h"
#include <time.h>

class LedClock
{
public:
	enum class Error {
		TIME_NOT_SET,
		HARDWARE_FAULT
	};
	LedClock(PeripheralReference<WS2812<60 + 12>> leds, PeripheralReference<LedPowerEnable> power);
	~LedClock();
	void displayTime(const tm &time, bool displaySeconds  = false, uint16_t subsecond = 0);
	void displayError(Error error);
	void setPower(bool on);
	bool isReady();
	void setBrightness(uint8_t brightness);
protected:
	PeripheralReference<WS2812<60 + 12>> mLeds;
	PeripheralReference<LedPowerEnable> mPowerEnable;
	uint8_t mBrightness;
};


#endif /* INC_LEDCLOCK_H_ */
