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
#include "Buttons.h"
#include "LedClockTask.h"
#include "Dcf77DecodeTask.h"

class SystemEventType : public EventType
{
public:
	enum type : uint32_t
	{
		DCF_POWER_DOWN,
		DCF_PULSE,
		DCF_WAKE_UP,
		SYSTICK_EVENT,
		DMA_BUFFER_EMPTY,
		POWER_SOURCE_CHANGED,
		SUPPLY_VOLTAGE_LEVEL,
		BUTTON_EVENT
	};
};

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
	Buttons mButtons;
	// Callbacks
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_PULSE)> mDcfPulseCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::POWER_SOURCE_CHANGED)> mPowerSourceCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_POWER_DOWN)> mDcfPowerDownCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_WAKE_UP)> mDcfWakeUpCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)> mSysTickCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SUPPLY_VOLTAGE_LEVEL)> mPowerSupplyCallback;
	// Tasks
	LedClockTask mLedClockTask;
	Dcf77DecodeTask mDcf77DecodeTask;
};

#endif /* INC_SYSTEMMANAGER_H_ */
