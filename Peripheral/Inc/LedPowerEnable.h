/*
 * LedPowerEnable.h
 *
 *  Created on: 11.07.2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_LEDPOWERENABLE_H_
#define PERIPHERAL_INC_LEDPOWERENABLE_H_

#include "Peripheral.h"

class LedPowerEnable : public Peripheral
{
public:
	LedPowerEnable();
	virtual void initialize();
	virtual void shutdown();
	void setPower(bool on);
};


#endif /* PERIPHERAL_INC_LEDPOWERENABLE_H_ */
