/*
 * PeripheralReference.h
 *
 *  Created on: 08.08.2019
 *      Author: timm
 */

#ifndef INC_PERIPHERALREFERENCE_H_
#define INC_PERIPHERALREFERENCE_H_

#include "Peripheral.h"

template<class periph>
class PeripheralReference: protected PeripheralInitializer
{
public:
	PeripheralReference(periph& peripheral);
	PeripheralReference(const periph& peripheral);
	PeripheralReference(const PeripheralReference& copy);
	periph& getInstance() const;
	void init();
	void shutdown();
	virtual ~PeripheralReference();
protected:
	bool mInitializationRequested = false;
	static uint8_t mInitializationCounter;
	periph& mReference;
};

#endif /* INC_PERIPHERALREFERENCE_H_ */
