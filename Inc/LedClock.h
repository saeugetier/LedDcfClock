/*
 * LedClock.h
 *
 *  Created on: 04.07.2019
 *      Author: timm
 */

#ifndef INC_LEDCLOCK_H_
#define INC_LEDCLOCK_H_

#include "main.h"
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
	LedClock(Settings* settings);
	~LedClock();
	void enable(bool enable);
	void displayTime(const tm &time, bool displaySeconds  = false, uint8_t subsecond = 0);
	void displayError(Error error);
	void setPower(bool on);
	bool isReady();
protected:
	WS2812<60 + 12> mLeds;
	//LedPowerEnable mLedPowerEnable;
	Settings* mSettings;
};


#endif /* INC_LEDCLOCK_H_ */
