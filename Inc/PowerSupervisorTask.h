/*
 * PowerSupervisorTask.h
 *
 *  Created on: 08.08.2019
 *      Author: timm
 */

#ifndef INC_POWERSUPERVISORTASK_H_
#define INC_POWERSUPERVISORTASK_H_

#include "Task.h"
#include "Callback.h"
#include "PeripheralReference.h"
#include "PowerSource.h"
#include "SupplyVoltageSupervisor.h"
#include "Event.h"


class PowerSupervisorTask : public Task
{
public:
	PowerSupervisorTask(PeripheralReference<PowerSource> source,
			PeripheralReference<SupplyVoltageSupervisor> supervisor,
			Callback* undervoltageCallback);
	virtual void setup();
	virtual void run();
	virtual void handleEvent(EventType::type event);
	virtual EventList getEvents();
protected:
	virtual void taskModeChanged(TaskMode mode);
	Callback* mUndervoltageCallback;
	PeripheralReference<PowerSource> mPowerSource;
	PeripheralReference<SupplyVoltageSupervisor> mSupervisor;

	Event mSupplyVoltageLevelEvent;
	Event mPowerSourceChanged;
};


#endif /* INC_POWERSUPERVISORTASK_H_ */
