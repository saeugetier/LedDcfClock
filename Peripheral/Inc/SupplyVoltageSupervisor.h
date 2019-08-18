#ifndef PERIPHERAL_INC_SUPPLYVOLTAGESUPERVISOR_H_
#define PERIPHERAL_INC_SUPPLYVOLTAGESUPERVISOR_H_

#include "Peripheral.h"

class SupplyVoltageSupervisor : public InterruptPeripheral<SupplyVoltageSupervisor>
{
protected:
	virtual void initialize();
	virtual void shutdown();
public:
	virtual void handleInterrupt();
};

#endif
