/*
 * SystemManager.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "SystemManager.h"

SystemManager::SystemManager() :
		mPulseDetector(false),
		mDcfPulseCallback(mTaskManager),
		mPowerSourceCallback(mTaskManager),
		mDcfPowerDownCallback(mTaskManager),
		mDcfWakeUpCallback(mTaskManager),
		mSysTickCallback(mTaskManager),
		mPowerSupplyCallback(mTaskManager),
		mLedClockTask(mClock, mWS2812, mPushButton, mSystemTick, mSettings),
		mDcf77DecodeTask(mPulseDetector, mClock, mDcfWakeup, mDcfPowerdown)
{
	mPulseDetector.getInstance().registerCallback(&mDcfPulseCallback);
	mDcfWakeup.getInstance().registerCallback(&mDcfWakeUpCallback);
	mSystemTick.getInstance().registerCallback(&mSysTickCallback);
	mDcfPowerdown.getInstance().registerCallback(&mDcfPowerDownCallback);

	mTaskManager.addTask(&mDcf77DecodeTask);
	mTaskManager.addTask(&mLedClockTask);

	mTaskManager.addEvent(&(mLedClockTask.getTickEvent()));

	mTaskManager.addEvent(&(mDcf77DecodeTask.getWakeupEvent()));
	mTaskManager.addEvent(&(mDcf77DecodeTask.getPowerdownEvent()));
	mTaskManager.addEvent(&(mDcf77DecodeTask.getPulseEvent()));

	mSettings1Button.init();
	mSettings2Button.init();
}

void SystemManager::runTasks()
{
	mTaskManager.runTasks();
}
