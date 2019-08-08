/*
 * Peripheral.h
 *
 *  Created on: May 30, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_PERIPHERAL_H_
#define PERIPHERAL_INC_PERIPHERAL_H_

#include "Callback.h"
#include "stdint.h"

class Peripheral;

class PeripheralInitializer
{
public:
	void _init(Peripheral& periph);
	void _shutdown(Peripheral& periph);
};

class Peripheral
{
public:
	friend void PeripheralInitializer::_init(Peripheral& periph);
	friend void PeripheralInitializer::_shutdown(Peripheral& periph);
protected:
	virtual void initialize() = 0;
	virtual void shutdown() = 0;
};

class InterruptPeripheral : public Peripheral
{
public:
	void registerCallback(Callback* callback);
	virtual void handleInterrupt() = 0;
	static InterruptPeripheral* getInstance();
protected:
	Callback* mCallback;
	static InterruptPeripheral* mPeripheralInstance;
};

#endif /* PERIPHERAL_INC_PERIPHERAL_H_ */
