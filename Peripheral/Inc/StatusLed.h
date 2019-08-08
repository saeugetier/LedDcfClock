/*
 * StatusLeds.h
 *
 *  Created on: Aug 8, 2019
 *      Author: timm
 */

#ifndef INC_STATUSLED_H_
#define INC_STATUSLED_H_

#include "Peripheral.h"

class StatusLed : public Peripheral
{
public:
	void setLed(bool on);
protected:
	virtual void initialize();
	virtual void shutdown();
};


#endif /* INC_STATUSLED_H_ */
