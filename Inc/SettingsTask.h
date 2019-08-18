/*
 * SettingsTask.h
 *
 *  Created on: 08.08.2019
 *      Author: timm
 */

#ifndef INC_SETTINGSTASK_H_
#define INC_SETTINGSTASK_H_

#include "Task.h"
#include "PeripheralReference.h"
#include "Buttons.h"
#include "Settings.h"
#include "Callback.h"
#include "Event.h"

class SettingsTask : public Task
{
public:
	SettingsTask(PeripheralReference<Settings1Button> settings1Button,
				PeripheralReference<Settings2Button> settings2Button,
				Callback* settingsChangeCallback, Settings* settings);
	virtual void run();
	virtual void handleEvent(EventType::type event);
	virtual EventList getEvents();
protected:
	virtual void taskModeChanged(TaskMode mode);
	PeripheralReference<Settings1Button> mButton1;
	PeripheralReference<Settings2Button> mButton2;
	Event mSettings1ButtonEvent;
	Event mSettings2ButtonEvent;
	Callback* mSettingsChangedCallback;
	Settings* mSettings;
};



#endif /* INC_SETTINGSTASK_H_ */
