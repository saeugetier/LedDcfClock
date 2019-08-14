/*
 * SystemTick.cpp
 *
 *  Created on: 31.07.2019
 *      Author: timm
 */

#include "SystemTick.h"
#include "main.h"

template<>
SystemTick* InterruptPeripheral<SystemTick>::mPeripheralInstance = nullptr;

SystemTick::SystemTick()
{
	mPeripheralInstance = this;
}

void SystemTick::initialize()
{
	LL_SYSTICK_EnableIT();
}

void SystemTick::shutdown()
{
	LL_SYSTICK_DisableIT();
}

void SystemTick::handleInterrupt()
{
	if(mCallback != nullptr)
		mCallback->notify();
}
