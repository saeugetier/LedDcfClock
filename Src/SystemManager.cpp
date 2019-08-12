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
				mTaskManager), mDcf77DecodeTask(mPulseDetector, mClock,
				mDcfWakeup, mDcfPowerdown), mLedClockTask(mClock, mWS2812,
				mPushButton, mSystemTick)
{
	mPulseDetector.getInstance().registerCallback(&mDcfPulseCallback);
	mDcfWakeup.getInstance().registerCallback(&mDcfWakeUpCallback);
	mDcfPowerdown.getInstance().registerCallback(&mDcfPowerDownCallback);
	mSystemTick.getInstance().registerCallback(&mSysTickCallback);

	mTaskManager.addTask(&mDcf77DecodeTask);
	mTaskManager.addTask(&mLedClockTask);

	mTaskManager.addEvent(&(mLedClockTask.getTickEvent()));

	mTaskManager.addEvent(&(mDcf77DecodeTask.getWakeupEvent()));
	mTaskManager.addEvent(&(mDcf77DecodeTask.getPowerdownEvent()));
	mTaskManager.addEvent(&(mDcf77DecodeTask.getPulseEvent()));
}

void SystemManager::runTasks()
{
	mTaskManager.runTasks();
}
