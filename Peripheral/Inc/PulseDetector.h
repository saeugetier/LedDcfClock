#ifndef PERIPHERAL_INC_PULSEDETECTOR_H_
#define PERIPHERAL_INC_PULSEDETECTOR_H_

#include "Peripheral.h"
#include "Callback.h"
#include "stdint.h"

class PulseDetector : public InterruptPeripheral<PulseDetector>
{
public:
	PulseDetector(bool SyncOnRisingFlank);
	void handleInterrupt();
	uint32_t getLowEdge();
	uint32_t getHighEdge();
protected:
	virtual void initialize();
	virtual void shutdown();

	Callback* mPulseCallback;
	Callback* mWakeupCallback;

	uint32_t mEdgeHigh;
	uint32_t mEdgeLow;

	bool mSyncOnRisingFlank;
};

#endif /* PERIPHERAL_INC_PULSEDETECTOR_H_ */
