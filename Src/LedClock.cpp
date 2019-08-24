/*
 * LedClock.cpp
 *
 *  Created on: 04.07.2019
 *      Author: timm
 */

#include "LedClock.h"
#ifdef DEBUG
#include "SEGGER_RTT.h"
#endif

LedClock::LedClock(PeripheralReference<WS2812<60 + 12>> leds, PeripheralReference<LedPowerEnable> power) :
	mLeds(leds), mPowerEnable(power), mBrightness(255)
{
}

LedClock::~LedClock()
{
	setPower(false);
}

void LedClock::displayTime(const tm &time, bool displaySeconds, uint8_t subsecond)
{
	static int last_second = 0;
	mLeds.getInstance().clearBuffer();

	//correct phase by 180°
	int second = (time.tm_sec  + 30) % 60;
	int minute = (time.tm_min + 30) % 60;
	int hour = (time.tm_hour + 6) % 12;

	if(subsecond > 128)
		subsecond = 128;

	if(last_second != second)
	{
		SEGGER_RTT_printf(0, "second: %d\r\n", second);
		last_second = second;
	}

	int brightness_subsecond = ((int)subsecond * mBrightness) / 128;

	//second and minutes are using the inner LEDs
	mLeds.getInstance().setPixelColor(RGB(0,0,brightness_subsecond), second);
	mLeds.getInstance().setPixelColor(RGB(0,0,mBrightness-brightness_subsecond), (second + 1) % 60);
	mLeds.getInstance().setPixelColor(RGB(mBrightness,0,mLeds.getInstance().getPixelColor(minute).b), minute);
	//hours using the uppper LED ring
	mLeds.getInstance().setPixelColor(RGB(0,mBrightness,0), 60 + hour);
	mLeds.getInstance().showLeds();
}


void LedClock::setPower(bool on)
{
	mPowerEnable.getInstance().setPower(on);
}

bool LedClock::isReady()
{
	return !mLeds.getInstance().isBusy();
}

void LedClock::displayError(Error error)
{
	mLeds.getInstance().clearBuffer();
	if(error == Error::HARDWARE_FAULT)
	{
		mLeds.getInstance().setPixelColor(RGB(0,128,128), 0);
		mLeds.getInstance().setPixelColor(RGB(0,128,128), 20);
		mLeds.getInstance().setPixelColor(RGB(0,128,128), 40);
	}
	else if(error == Error::TIME_NOT_SET)
	{
		mLeds.getInstance().setPixelColor(RGB(128,128,0), 0);
		mLeds.getInstance().setPixelColor(RGB(128,128,0), 20);
		mLeds.getInstance().setPixelColor(RGB(128,128,0), 40);
	}
	mLeds.getInstance().showLeds();
}

void LedClock::setBrightness(uint8_t brightness)
{
	mBrightness = brightness;
}
