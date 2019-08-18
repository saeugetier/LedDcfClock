/*
 * SettingsStorage.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: timm
 */

#include "SettingsStorage.h"
#include "stm32g0xx_ll_rtc.h"
#include "stm32g0xx_ll_bus.h"
#include "assert.h"

void SettingsStorage::initialize()
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);
}

void SettingsStorage::shutdown()
{

}

uint32_t SettingsStorage::readAddress(uint8_t address)
{
	assert(address >= LL_RTC_BKP_DR0 && address <= LL_RTC_BKP_DR4);

	return LL_RTC_BKP_GetRegister(TAMP, address);
}

void SettingsStorage::writeAddress(uint8_t address, uint32_t data)
{
	assert(address >= LL_RTC_BKP_DR0 && address <= LL_RTC_BKP_DR4);

	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_BKP_SetRegister(TAMP, address, data);
	LL_RTC_EnableWriteProtection(RTC);
}
