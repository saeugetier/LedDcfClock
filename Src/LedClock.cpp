/*
 * LedClock.cpp
 *
 *  Created on: 04.07.2019
 *      Author: timm
 */

#include "LedClock.h"

LedClock::LedClock(Settings* settings)
{
	mSettings = settings;
}

LedClock::~LedClock()
{
	enable(false);
}

void LedClock::enable(bool enable)
{
	if(enable)
	{
		mLedPowerEnable.initialize();
		//mLeds.initialize();
	}
	else
	{
		setPower(false);
		mLedPowerEnable.shutdown();
		//mLeds.shutdown();
	}
}

void LedClock::displayTime(const tm &time, bool displaySeconds, uint8_t subsecond)
{
	mLeds.clearBuffer();

	//correct phase by 180°
	int second = (time.tm_sec  + 30) % 60;
	int minute = (time.tm_min + 30) % 60;
	int hour = (time.tm_hour + 6) % 12;

	uint8_t brightness = mSettings->getBrightness();

	if(subsecond > 128)
		subsecond = 128;

	int brightness_subsecond = ((int)subsecond * brightness) / 128;

	//second and minutes are using the inner LEDs
	mLeds.setPixelColor(RGB(0,0,brightness_subsecond), second);
	mLeds.setPixelColor(RGB(0,0,brightness-brightness_subsecond), (second + 1) % 60);
	mLeds.setPixelColor(RGB(brightness,0,mLeds.getPixelColor(minute).b), minute);
	//hours using the uppper LED ring
	mLeds.setPixelColor(RGB(0,brightness,0), 60 + hour);
	mLeds.showLeds();
}


void LedClock::setPower(bool on)
{
	mLedPowerEnable.setPower(on);
}

bool LedClock::isReady()
{
	return !mLeds.isBusy();
}

void LedClock::displayError(Error error)
{
	mLeds.clearBuffer();
	if(error == Error::HARDWARE_FAULT)
	{
		mLeds.setPixelColor(RGB(0,255,255), 0);
		mLeds.setPixelColor(RGB(0,255,255), 1);
		mLeds.setPixelColor(RGB(0,255,255), 2);
	}
	else if(error == Error::TIME_NOT_SET)
	{
		mLeds.setPixelColor(RGB(255,255,0), 0);
		mLeds.setPixelColor(RGB(255,255,0), 1);
		mLeds.setPixelColor(RGB(255,255,0), 2);
	}
	mLeds.showLeds();
}
