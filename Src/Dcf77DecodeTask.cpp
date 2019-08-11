/*
 * Dcf77DecodeTask.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "Dcf77DecodeTask.h"
#include "SystemManager.h"

Dcf77DecodeTask::Dcf77DecodeTask(PeripheralReference<PulseDetector> detector,
		PeripheralReference<RtcClock> clock,
		PeripheralReference<DcfWakeup> wake,
		PeripheralReference<DcfPowerdown> power) :
		mPulseDetector(detector), mDcfWakeup(wake), mPowerDown(power), mRtcClock(
				clock)
{
	mDcfWakeup.init();
	setTaskMode(TaskMode::DEEPSLEEP);
}

void Dcf77DecodeTask::run()
{

}

void Dcf77DecodeTask::handleEvent(EventType::type event)
{
	switch (event)
	{
	case SystemEventType::type::DCF_WAKE_UP:
		break;
	}
}

void Dcf77DecodeTask::taskModeChanged(TaskMode mode)
{

}
