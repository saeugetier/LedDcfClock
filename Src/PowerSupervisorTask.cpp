/*
 * PowerSupervisorTask.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: timm
 */

#include "PowerSupervisorTask.h"
#include "SystemManager.h"

PowerSupervisorTask::PowerSupervisorTask(PeripheralReference<PowerSource> source,
		PeripheralReference<SupplyVoltageSupervisor> supervisor,
		Callback* undervoltageCallback) :
	mUndervoltageCallback(undervoltageCallback), mPowerSource(source), mSupervisor(supervisor),
	mSupplyVoltageLevelEvent(this, static_cast<EventType::type>(SystemEventType::SUPPLY_VOLTAGE_LEVEL)),
	mPowerSourceChanged(this, static_cast<EventType::type>(SystemEventType::POWER_SOURCE_CHANGED))
{

}

void PowerSupervisorTask::setup()
{
	setTaskMode(TaskMode::DEEPSLEEP);
}

void PowerSupervisorTask::run()
{

}

void PowerSupervisorTask::handleEvent(EventType::type event)
{
	SystemEventType::type _event = static_cast<SystemEventType::type>(event);

	switch(_event)
	{
	case SystemEventType::SUPPLY_VOLTAGE_LEVEL:
	case SystemEventType::POWER_SOURCE_CHANGED:
		mUndervoltageCallback->notify();
		break;
	default:
		break;
	}
}

void PowerSupervisorTask::taskModeChanged(TaskMode mode)
{
	if(mode == TaskMode::DEEPSLEEP)
	{
		mPowerSource.init();
	}
}

EventList PowerSupervisorTask::getEvents()
{
	EventList list;
	list.push_front(&mSupplyVoltageLevelEvent);
	list.push_front(&mPowerSourceChanged);
	return list;
}
