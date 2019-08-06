/*
 * Peripheral.cpp
 *
 *  Created on: 01.08.2019
 *      Author: timm
 */

#include "Peripheral.h"

InterruptPeripheral* InterruptPeripheral::mPeripheralInstance = nullptr;

void PeripheralInitializer::_init(Peripheral &periph)
{
	periph.initialize();
}

void PeripheralInitializer::_shutdown(Peripheral &periph)
{
	periph.shutdown();
}

void InterruptPeripheral::registerCallback(Callback* callback)
{
	mCallback = callback;
	mPeripheralInstance = this;
}

InterruptPeripheral* InterruptPeripheral::getInstance()
{
	return mPeripheralInstance;
}
