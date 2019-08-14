/*
 * Peripheral.cpp
 *
 *  Created on: 01.08.2019
 *      Author: timm
 */

#include "Peripheral.h"


void PeripheralInitializer::_init(Peripheral &periph)
{
	periph.initialize();
}

void PeripheralInitializer::_shutdown(Peripheral &periph)
{
	periph.shutdown();
}
