#ifndef PERIPHERAL_INC_CLOCK_H_
#define PERIPHERAL_INC_CLOCK_H_

#include "Peripheral.h"
#include "time.h"
#include "Settings.h"

class RtcClock : public Peripheral
{
public:
	virtual void initialize();
	virtual void shutdown();
	time_t now();
	uint16_t getSubsecond();
	void calibrate(time_t time, uint16_t subseconds);
	bool setTime(time_t time, uint16_t subseconds);
	bool isClockSet();
protected:
	Settings* mSettings;
};


#endif /* PERIPHERAL_INC_CLOCK_H_ */
