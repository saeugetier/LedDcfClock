/*,
 * Dcf77DecodeTask.h
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#ifndef INC_DCF77DECODETASK_H_
#define INC_DCF77DECODETASK_H_

#include "Task.h"
#include "PeripheralReference.h"
#include "PulseDetector.h"
#include "Clock.h"
#include "DcfWakeup.h"
#include "DcfPowerdown.h"
#include "Dcf77.h"
#include "Event.h"

class Dcf77DecodeTask : public Task
{
public:
	Dcf77DecodeTask(PeripheralReference<PulseDetector> detector, PeripheralReference<RtcClock> clock, PeripheralReference<DcfWakeup> wake, PeripheralReference<DcfPowerdown> power);
	virtual void run();
	virtual void handleEvent(EventType::type event);
	Event& getWakeupEvent() {return mDcfWakeupEvent;}
	Event& getPowerdownEvent() {return mDcfPowerdownEvent;}
	Event& getPulseEvent() {return mDcfPulseEvent;}
protected:
	virtual void taskModeChanged(TaskMode mode);
	PeripheralReference<PulseDetector> mPulseDetector;
	PeripheralReference<RtcClock> mRtcClock;
	PeripheralReference<DcfWakeup> mDcfWakeup;
	PeripheralReference<DcfPowerdown> mDcfPowerDown;
	DCF77 mDcfDecoder;

	Event mDcfWakeupEvent;
	Event mDcfPowerdownEvent;
	Event mDcfPulseEvent;
};

#endif /* INC_DCF77DECODETASK_H_ */
