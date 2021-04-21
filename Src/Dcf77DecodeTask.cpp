/*
 * Dcf77DecodeTask.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: timm
 */

#include "Dcf77DecodeTask.h"
#include "SystemManager.h"
#ifdef DEBUG
#include "SEGGER_RTT.h"
#endif

Dcf77DecodeTask::Dcf77DecodeTask(PeripheralReference<PulseDetector> detector,
		PeripheralReference<RtcClock> clock,
		PeripheralReference<DcfWakeup> wake,
		PeripheralReference<DcfPowerdown> power) :
		mPulseDetector(detector),
		mRtcClock(clock),
		mDcfWakeup(wake),
		mDcfPowerDown(power),
		mDcfDecoder(mRtcClock, false),
		mDcfWakeupEvent(this, static_cast<EventType::type>(SystemEventType::DCF_WAKE_UP)),
		mDcfPowerdownEvent(this, static_cast<EventType::type>(SystemEventType::DCF_POWER_DOWN)),
		mDcfPulseEvent(this, static_cast<EventType::type>(SystemEventType::DCF_PULSE)),
		mDcfTimeoutEvent(this, static_cast<EventType::type>(SystemEventType::DCF_TIMEOUT))
{

}

void Dcf77DecodeTask::setup()
{
	setTaskMode(TaskMode::DEEPSLEEP);
}

void Dcf77DecodeTask::run()
{

}

void Dcf77DecodeTask::handleEvent(EventType::type event)
{
	SystemEventType::type _event = static_cast<SystemEventType::type>(event);
	time_t time;

	uint32_t high;
	uint32_t low;
	switch (_event)
	{
	case SystemEventType::type::DCF_WAKE_UP:
		setTaskMode(TaskMode::SLEEP);
#ifdef DEBUG
		SEGGER_RTT_printf(0, "wake up\r\n");
#endif
		break;
	case SystemEventType::type::DCF_PULSE:
		high = mPulseDetector.getInstance().getHighEdge();
		low = mPulseDetector.getInstance().getLowEdge();
#ifdef DEBUG
		SEGGER_RTT_printf(0, "low: %u\r\n", low);
		SEGGER_RTT_printf(0, "high: %u\r\n", high);
		SEGGER_RTT_printf(0, "diff: %u\r\n", high-low);
#endif
		mDcfDecoder.decode(high,low);
		time = mDcfDecoder.getTime();
		// time != 0 when new timestamp is available
		if(time != 0)
		{
#ifdef DEBUG
			SEGGER_RTT_printf(0, "synced\r\n");
#endif
			if(!mRtcClock.getInstance().isClockSet())
				mRtcClock.getInstance().setTime(time, mDcfDecoder.getMedianSubsecond());
			else
				mRtcClock.getInstance().calibrate(time, mDcfDecoder.getMedianSubsecond());
		}
		break;
	case SystemEventType::type::DCF_TIMEOUT:
	case SystemEventType::type::DCF_POWER_DOWN:
#ifdef DEBUG
		SEGGER_RTT_printf(0, "power down\r\n");
		for(int i = 0; i < 3000; i++);
#endif
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
	case TaskMode::WAKE:
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

EventList Dcf77DecodeTask::getEvents()
{
	EventList list;
	list.push_front(&mDcfWakeupEvent);
	list.push_front(&mDcfPowerdownEvent);
	list.push_front(&mDcfPulseEvent);
	list.push_front(&mDcfTimeoutEvent);
	return list;
}
