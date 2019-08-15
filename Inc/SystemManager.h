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
#include "PeripheralReference.h"
//Peripheral
#include "SystemTick.h"
#include "LedPowerEnable.h"
#include "WS2812.h"
#include "PulseDetector.h"
#include "DcfWakeup.h"
#include "DcfPowerdown.h"
#include "Clock.h"
#include "Buttons.h"
#include "LedClockTask.h"
#include "Dcf77DecodeTask.h"
#include "StatusLed.h"
#include "PowerSupervisorTask.h"
#include "SettingsTask.h"
#include "PowerSource.h"

class SystemEventType : public EventType
{
public:
	enum type : uint32_t
	{
		NONE = 0,
		DCF_POWER_DOWN,
		DCF_PULSE,
		DCF_WAKE_UP,
		SYSTICK_EVENT,
		POWER_SOURCE_CHANGED,
		SUPPLY_VOLTAGE_LEVEL,
		PUSH_BUTTON_EVENT,
		SETTINGS1_BUTTON_EVENT,
		SETTINGS2_BUTTON_EVENT,
		CLOCK_ALARM_EVENT,
		SETTINGS_CHANGED,
		UNDERVOLTAGE_SHUTDOWN
	};
};

class SystemManager
{
public:
	SystemManager();
	void runTasks();
protected:
	TaskManager mTaskManager;
	//Peripherals
	//LED
	PeripheralReference<SystemTick> mSystemTick = SystemTick();
	PeripheralReference<WS2812<60+12>> mWS2812 = WS2812<60+12>();
	PeripheralReference<LedPowerEnable> mLedPowerEnable = LedPowerEnable();
	//DCF
	PeripheralReference<PulseDetector> mPulseDetector = PulseDetector(false);
	PeripheralReference<DcfWakeup> mDcfWakeup = DcfWakeup();
	PeripheralReference<DcfPowerdown> mDcfPowerdown = DcfPowerdown();
	//Clock
	PeripheralReference<RtcClock> mClock = RtcClock();
	//Common
	PeripheralReference<StatusLed1> mLed1 = StatusLed1();
	PeripheralReference<StatusLed2> mLed2 = StatusLed2();
	PeripheralReference<PowerSource> mPowerSource = PowerSource();
    //HMI
	PeripheralReference<PushButton> mPushButton = PushButton();
	PeripheralReference<Settings1Button> mSettings1Button = Settings1Button();
	PeripheralReference<Settings2Button> mSettings2Button = Settings2Button();

	// Callbacks Peripheral
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_POWER_DOWN)> mDcfPowerDownCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_PULSE)> mDcfPulseCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::DCF_WAKE_UP)> mDcfWakeUpCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SYSTICK_EVENT)> mSysTickCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::POWER_SOURCE_CHANGED)> mPowerSourceCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SUPPLY_VOLTAGE_LEVEL)> mPowerSupplyCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::PUSH_BUTTON_EVENT)> mPushButtonCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SETTINGS1_BUTTON_EVENT)> mSettings1ButtonCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::SETTINGS2_BUTTON_EVENT)> mSettings2ButtonCallback;
	// Callback Tasks
	TaskCallback<static_cast<EventType::type>(SystemEventType::SETTINGS_CHANGED)> mSettingsChangedCallback;
	TaskCallback<static_cast<EventType::type>(SystemEventType::UNDERVOLTAGE_SHUTDOWN)> mUnderVoltageCallback;
	// Tasks
	LedClockTask mLedClockTask;
	Dcf77DecodeTask mDcf77DecodeTask;
	PowerSupervisorTask mPowerSupervisorTask;

	Settings mSettings;
};

#endif /* INC_SYSTEMMANAGER_H_ */
