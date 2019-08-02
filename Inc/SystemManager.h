/*
 * SystemManager.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_SYSTEMMANAGER_H_
#define INC_SYSTEMMANAGER_H_

#include "TaskManager.h"

#include "Peripheral.h"
//Peripheral
#include "SystemTick.h"
#include "LedPowerEnable.h"
#include "WS2812.h"
#include "PulseDetector.h"
#include "Clock.h"

template<class periph>
class PeripheralReference: protected PeripheralInitializer
{
public:
	PeripheralReference(periph& peripheral);
	PeripheralReference(PeripheralReference& copy);
	periph& getInstance() const;
	void init();
	void shutdown();
	virtual ~PeripheralReference();
protected:
	bool mInitializationRequested = false;
	static uint8_t mInitializationCounter;
	periph& mReference;
};

class SystemManager
{
public:
	SystemManager();
	void runTasks();
	PeripheralReference<SystemTick> getSystemTickReference();
	PeripheralReference<WS2812<60+12>> getWS2812Reference();
	PeripheralReference<LedPowerEnable> getLedPowerEnableReference();
	PeripheralReference<PulseDetector> getPulseDetector();
	PeripheralReference<RtcClock> getClockReference();
protected:
	TaskManager mTaskManager;
	//Peripherals
	SystemTick mSystemTick;
	WS2812<60+12> mWS2812;
	LedPowerEnable mLedPowerEnable;
	PulseDetector mPulseDetector;
	RtcClock mClock;
	//Callbacks
	TaskCallback<EventType::DCF_PULSE> mDcfPulseCallback;
};

#endif /* INC_SYSTEMMANAGER_H_ */
