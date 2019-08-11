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
	mPulseDetector.getInstance().registerCallback(&mDcfPulseCallback);
	mDcfWakeup.getInstance().registerCallback(&mDcfWakeUpCallback);
	mDcfPowerdown.getInstance().registerCallback(&mDcfPowerDownCallback);
	mSystemTick.getInstance().registerCallback(&mSysTickCallback);

	mTaskManager.addTask(&mDcf77DecodeTask);
	mTaskManager.addTask(&mLedClockTask);
}

void SystemManager::runTasks()
{
	mTaskManager.runTasks();
}
