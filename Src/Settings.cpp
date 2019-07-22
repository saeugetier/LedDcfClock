/*
 * Settings.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: timm
 */

#include "Settings.h"

/* Backup registers are use, because they are not reset in standby mode */

uint8_t Settings::getBrightness()
{
	return LL_RTC_BKP_GetRegister(TAMP, LL_RTC_BKP_DR0) & 0xFF;
}

uint8_t Settings::getMode()
{
	return LL_RTC_BKP_GetRegister(TAMP, LL_RTC_BKP_DR1) & 0xFF;
}

void Settings::setBrightness(uint8_t brightness)
{
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_BKP_SetRegister(TAMP, LL_RTC_BKP_DR0, brightness);
	LL_RTC_EnableWriteProtection(RTC);
}

void Settings::setMode(uint8_t mode)
{
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_BKP_SetRegister(TAMP, LL_RTC_BKP_DR1, mode);
	LL_RTC_EnableWriteProtection(RTC);
}

uint32_t Settings::getLastCalibrationTimestamp()
{
	return LL_RTC_BKP_GetRegister(TAMP, LL_RTC_BKP_DR3);
}

void Settings::setLastCalibrationTimestamp(uint32_t timeStamp)
{
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_BKP_SetRegister(TAMP, LL_RTC_BKP_DR3, timeStamp);
	LL_RTC_EnableWriteProtection(RTC);
}

