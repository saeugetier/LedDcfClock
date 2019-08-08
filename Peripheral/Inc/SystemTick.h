/*
 * SystemTick.h
 *
 *  Created on: 31.07.2019
 *      Author: timm
 */

#ifndef INC_SYSTEMTICK_H_
#define INC_SYSTEMTICK_H_

#include "Peripheral.h"

class SystemTick : public InterruptPeripheral
{
public:
	void handleInterrupt();
protected:
	virtual void initialize();
	virtual void shutdown();
};


#endif /* INC_SYSTEMTICK_H_ */
