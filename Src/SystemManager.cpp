/*
 * SystemManager.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "SystemManager.h"

SystemManager::SystemManager() :
		mPulseDetector(false), mDcfPulseCallback(mTaskManager), mPowerSourceCallback(
				mTaskManager), mDcfPowerDownCallback(mTaskManager), mDcfWakeUpCallback(
				mTaskManager), mSysTickCallback(mTaskManager), mPowerSupplyCallback(
				mTaskManager), mDcf77DecodeTask(mPulseDetector, mClock, mDcfWakeup, mDcfPowerdown)
{
	mPulseDetector.registerCallback(&mDcfPulseCallback);
	mDcfWakeup.registerCallback(&mDcfWakeUpCallback);
	mDcfPowerdown.registerCallback(&mDcfPowerDownCallback);
	mSystemTick.registerCallback(&mSysTickCallback);
}

void SystemManager::runTasks()
{
	mTaskManager.runTasks();
}

PeripheralReference<SystemTick> SystemManager::getSystemTickReference()
{
	PeripheralReference<SystemTick> reference(mSystemTick);
	return reference;
}

PeripheralReference<WS2812<60 + 12>> SystemManager::getWS2812Reference()
{
	PeripheralReference<WS2812<60 + 12>> reference(mWS2812);
	return reference;
}

PeripheralReference<LedPowerEnable> SystemManager::getLedPowerEnableReference()
{
	PeripheralReference<LedPowerEnable> reference(mLedPowerEnable);
	return reference;
}

PeripheralReference<PulseDetector> SystemManager::getPulseDetector()
{
	PeripheralReference<PulseDetector> reference(mPulseDetector);
	return reference;
}

PeripheralReference<DcfWakeup> SystemManager::getDcfWakeupReference()
{
	PeripheralReference<DcfWakeup> reference(mDcfWakeup);
	return reference;
}

PeripheralReference<DcfPowerdown> SystemManager::getDcfPowerdownReference()
{
	PeripheralReference<DcfPowerdown> reference(mDcfPowerdown);
	return reference;
}

PeripheralReference<RtcClock> SystemManager::getClockReference()
{
	PeripheralReference<RtcClock> reference(mClock);
	return reference;
}

PeripheralReference<StatusLed> SystemManager::getStatusLed1Reference()
{
	PeripheralReference<StatusLed> reference(mLed1);
	return reference;
}

PeripheralReference<StatusLed> SystemManager::getStatusLed2Reference()
{
	PeripheralReference<StatusLed> reference(mLed2);
	return reference;
}
