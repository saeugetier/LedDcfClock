/*
 * Peripheral.h
 *
 *  Created on: May 30, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_PERIPHERAL_H_
#define PERIPHERAL_INC_PERIPHERAL_H_

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

#endif /* PERIPHERAL_INC_PERIPHERAL_H_ */
