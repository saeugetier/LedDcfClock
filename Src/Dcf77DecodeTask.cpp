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
		mPulseDetector(detector),
		mRtcClock(clock),
		mDcfWakeup(wake),
		mDcfPowerDown(power),
		mDcfDecoder(&mRtcClock.getInstance(), false),
		mDcfWakeupEvent(this, static_cast<EventType::type>(SystemEventType::DCF_WAKE_UP)),
		mDcfPowerdownEvent(this, static_cast<EventType::type>(SystemEventType::DCF_POWER_DOWN)),
		mDcfPulseEvent(this, static_cast<EventType::type>(SystemEventType::DCF_PULSE))
{
	setTaskMode(TaskMode::DEEPSLEEP);
}

void Dcf77DecodeTask::run()
{
	time_t time = mDcfDecoder.getTime();
	// time != 0 when new timestamp is available
	if(time != 0)
	{
		if(!mRtcClock.getInstance().isClockSet())
			mRtcClock.getInstance().setTime(time, mDcfDecoder.getMedianSubsecond());
		else
			mRtcClock.getInstance().calibrate(time, mDcfDecoder.getMedianSubsecond());
	}
}

void Dcf77DecodeTask::handleEvent(EventType::type event)
{
	SystemEventType::type _event = static_cast<SystemEventType::type>(event);

	switch (_event)
	{
	case SystemEventType::type::DCF_WAKE_UP:
		setTaskMode(TaskMode::SLEEP);
		break;
	case SystemEventType::type::DCF_PULSE:
		mDcfDecoder.decode(mPulseDetector.getInstance().getHighEdge(), mPulseDetector.getInstance().getLowEdge());
		break;
	case SystemEventType::type::DCF_POWER_DOWN:
		setTaskMode(TaskMode::DEEPSLEEP);
		break;
	default:
		break;
	}
}

void Dcf77DecodeTask::taskModeChanged(TaskMode mode)
{
	switch(mode)
	{
	case TaskMode::SLEEP:
		mDcfWakeup.shutdown();
		mRtcClock.init();
		mDcfPowerDown.init();
		mPulseDetector.init();
		break;
	default:
	case TaskMode::DEEPSLEEP:
		mDcfPowerDown.shutdown();
		mPulseDetector.shutdown();
		mRtcClock.shutdown();
		mDcfWakeup.init();
		break;

	}
}
