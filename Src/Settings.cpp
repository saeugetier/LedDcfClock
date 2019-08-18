/*
 * Settings.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: timm
 */

#include "Settings.h"

/* Backup registers are use, because they are not reset in standby mode */

Settings::Settings(PeripheralReference<SettingsStorage> storage) : mStorage(storage)
{
	mStorage.init();
}

uint8_t Settings::getBrightness()
{
	return mStorage.getInstance().readAddress(0) & 0xFF;
}

uint8_t Settings::getMode()
{
	return mStorage.getInstance().readAddress(1) & 0xFF;
}

void Settings::setBrightness(uint8_t brightness)
{
	mStorage.getInstance().writeAddress(0, brightness);
}

void Settings::setMode(uint8_t mode)
{
	mStorage.getInstance().writeAddress(1, mode);
}

uint32_t Settings::getLastCalibrationTimestamp()
{
	return mStorage.getInstance().readAddress(3);
}

void Settings::setLastCalibrationTimestamp(uint32_t timeStamp)
{
	mStorage.getInstance().writeAddress(3, timeStamp);
}

