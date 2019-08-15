/*
 * LedClock.cpp
 *
 *  Created on: 04.07.2019
 *      Author: timm
 */

#include "LedClock.h"

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
	mLeds.getInstance().clearBuffer();

	//correct phase by 180°
	int second = (time.tm_sec  + 30) % 60;
	int minute = (time.tm_min + 30) % 60;
	int hour = (time.tm_hour + 6) % 12;

	if(subsecond > 128)
		subsecond = 128;

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
		mLeds.getInstance().setPixelColor(RGB(0,255,255), 0);
		mLeds.getInstance().setPixelColor(RGB(0,255,255), 20);
		mLeds.getInstance().setPixelColor(RGB(0,255,255), 40);
	}
	else if(error == Error::TIME_NOT_SET)
	{
		mLeds.getInstance().setPixelColor(RGB(255,255,0), 0);
		mLeds.getInstance().setPixelColor(RGB(255,255,0), 20);
		mLeds.getInstance().setPixelColor(RGB(255,255,0), 40);
	}
	mLeds.getInstance().showLeds();
}

void LedClock::setBrightness(uint8_t brightness)
{
	mBrightness = brightness;
}
