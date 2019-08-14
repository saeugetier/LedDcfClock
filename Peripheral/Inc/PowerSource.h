/*
 * PowerSource.h
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#ifndef INC_POWERSOURCE_H_
#define INC_POWERSOURCE_H_

#include "Peripheral.h"

class PowerSource : public InterruptPeripheral<PowerSource>
{
protected:
	PowerSource();
	virtual void initialize();
	virtual void shutdown();
public:
	virtual void handleInterrupt();
};

#endif /* INC_POWERSOURCE_H_ */
