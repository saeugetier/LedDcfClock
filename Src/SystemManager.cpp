/*
 * SystemManager.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "SystemManager.h"

SystemManager::SystemManager() :
		mPulseDetector(false),
		mDcfPowerDownCallback(mTaskManager),
		mDcfPulseCallback(mTaskManager),
		mDcfWakeUpCallback(mTaskManager),
		mSysTickCallback(mTaskManager),
		mPowerSourceCallback(mTaskManager),
		mPowerSupplyCallback(mTaskManager),
		mPushButtonCallback(mTaskManager),
		mSettings1ButtonCallback(mTaskManager),
		mSettings2ButtonCallback(mTaskManager),
		mSettingsChangedCallback(mTaskManager),
		mUnderVoltageCallback(mTaskManager),
		mLedClockTask(mClock, mWS2812, mPushButton, mSystemTick, mLedPowerEnable, mSettings),
		mDcf77DecodeTask(mPulseDetector, mClock, mDcfWakeup, mDcfPowerdown),
		mPowerSupervisorTask(mPowerSource, &mUnderVoltageCallback)
{
	//initialize callbacks
	mDcfPowerdown.getInstance().registerCallback(&mDcfPowerDownCallback);
	mPulseDetector.getInstance().registerCallback(&mDcfPulseCallback);
	mDcfWakeup.getInstance().registerCallback(&mDcfWakeUpCallback);
	mSystemTick.getInstance().registerCallback(&mSysTickCallback);
	mPowerSource.getInstance().registerCallback(&mPowerSourceCallback);
	mPushButton.getInstance().registerCallback(&mPushButtonCallback);
	mSettings1Button.getInstance().registerCallback(&mSettings1ButtonCallback);
	mSettings2Button.getInstance().registerCallback(&mSettings2ButtonCallback);

	//add tasks
	mTaskManager.addTask(&mDcf77DecodeTask);
	mTaskManager.addTask(&mLedClockTask);
}

void SystemManager::runTasks()
{
	mTaskManager.runTasks();
}
