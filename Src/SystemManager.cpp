/*
 * SystemManager.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "SystemManager.h"

template class PeripheralReference<SystemTick> ;

template<class periph>
uint8_t PeripheralReference<periph>::mInitializationCounter = 0;

template<class periph>
PeripheralReference<periph>::PeripheralReference(periph& peripheral) :
		mInitializationRequested(false), mReference(peripheral)
{
}

template<class periph>
PeripheralReference<periph>::PeripheralReference(PeripheralReference& copy) :
		mInitializationRequested(false), mReference(copy.mReference)
{
}

template<class periph>
void PeripheralReference<periph>::init()
{
	if (!mInitializationRequested)
	{
		if (mInitializationCounter == 0)
			_init(mReference);
		mInitializationCounter++;
	}
	mInitializationRequested = true;
}

template<class periph>
void PeripheralReference<periph>::shutdown()
{
	if (mInitializationRequested)
	{
		mInitializationCounter--;
		if (mInitializationCounter == 0)
			_shutdown(mReference);
	}
	mInitializationRequested = false;
}

template<class periph>
PeripheralReference<periph>::~PeripheralReference()
{
	shutdown();
}

SystemManager::SystemManager() :
		mPulseDetector(false), mDcfPulseCallback(mTaskManager), mPowerSourceCallback(
				mTaskManager), mDcfPowerDownCallback(mTaskManager), mDcfWakeUpCallback(
				mTaskManager), mSysTickCallback(mTaskManager), mPowerSupplyCallback(
				mTaskManager)
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

PeripheralReference<Buttons> SystemManager::getButtonsReference()
{
	PeripheralReference<Buttons> reference(mButtons);
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
