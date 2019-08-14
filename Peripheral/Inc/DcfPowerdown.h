/*
 * DcfPowerdown.h
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_DCFPOWERDOWN_H_
#define PERIPHERAL_INC_DCFPOWERDOWN_H_

#include "Peripheral.h"

class DcfPowerdown : public InterruptPeripheral<DcfPowerdown>
{
public:
	DcfPowerdown();
	void handleInterrupt();
protected:
	void initialize();
	void shutdown();
};

#endif /* PERIPHERAL_INC_DCFPOWERDOWN_H_ */
