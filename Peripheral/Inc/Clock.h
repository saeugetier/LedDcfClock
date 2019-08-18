#ifndef PERIPHERAL_INC_CLOCK_H_
#define PERIPHERAL_INC_CLOCK_H_

#include "Peripheral.h"
#include "time.h"
#include "Settings.h"

class RtcClock : public InterruptPeripheral<RtcClock>
{
public:
	enum Alarm
	{
		NONE,
		HOURLY,
		EVERY_15_MINUTES,
		EVERY_5_MINUTES
	};

	RtcClock();
	time_t now();
	uint16_t getSubsecond();
	void calibrate(time_t time, uint16_t subseconds);
	bool setTime(time_t time, uint16_t subseconds);
	bool isClockSet();
	void handleInterrupt();

	void setAlarmMode(Alarm);
protected:
	Settings* mSettings;
	void applyCorrectionValue(int32_t correction_value);
	virtual void initialize();
	virtual void shutdown();

	Alarm mCurrentAlarm;
};


#endif /* PERIPHERAL_INC_CLOCK_H_ */
