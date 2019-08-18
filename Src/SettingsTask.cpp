/*
 * SettingsTask.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: timm
 */

#include "SettingsTask.h"
#include "SystemManager.h"

SettingsTask::SettingsTask(PeripheralReference<Settings1Button> settings1Button,
				PeripheralReference<Settings2Button> settings2Button,
				Callback* settingsChangeCallback, Settings* settings) :
				mButton1(settings1Button), mButton2(settings2Button),
				mSettings1ButtonEvent(this, static_cast<EventType::type>(SystemEventType::SETTINGS1_BUTTON_EVENT)),
				mSettings2ButtonEvent(this, static_cast<EventType::type>(SystemEventType::SETTINGS1_BUTTON_EVENT)),
				mSettingsChangedCallback(settingsChangeCallback),
				mSettings(settings)
{
	setTaskMode(TaskMode::DEEPSLEEP);
}

void SettingsTask::run()
{

}

void SettingsTask::handleEvent(EventType::type event)
{
	SystemEventType::type _event = static_cast<SystemEventType::type>(event);
	switch(_event)
	{
	case SystemEventType::SETTINGS1_BUTTON_EVENT:
		break;
	case SystemEventType::SETTINGS2_BUTTON_EVENT:
		break;
	default:
		break;
	}
}

EventList SettingsTask::getEvents()
{
	EventList list;
	list.push_front(&mSettings1ButtonEvent);
	list.push_front(&mSettings2ButtonEvent);
	return list;
}

void SettingsTask::taskModeChanged(TaskMode mode)
{
	if(mode == TaskMode::DEEPSLEEP)
	{
		mButton1.init();
		mButton2.init();
	}
}
