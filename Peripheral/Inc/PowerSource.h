/*
 * PowerSource.h
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#ifndef INC_POWERSOURCE_H_
#define INC_POWERSOURCE_H_

#include "Peripheral.h"

class PowerSource : public Peripheral
{
protected:
	virtual void initialize();
	virtual void shutdown();
};

#endif /* INC_POWERSOURCE_H_ */
