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
		mPulseDetector(false), mDcfPulseCallback(mTaskManager),
		mPowerSourceCallback(mTaskManager)
{
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
