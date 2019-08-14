/*
 * DcfWakeup.h
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_DCFWAKEUP_H_
#define PERIPHERAL_INC_DCFWAKEUP_H_

#include "Peripheral.h"

class DcfWakeup : public InterruptPeripheral<DcfWakeup>
{
public:
	DcfWakeup();
	void handleInterrupt();
	void clearWakeupFlags();
protected:
	void initialize();
	void shutdown();
};



#endif /* PERIPHERAL_INC_DCFWAKEUP_H_ */
