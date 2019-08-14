/*
 * PowerSource.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#include "PowerSource.h"

template<>
PowerSource* InterruptPeripheral<PowerSource>::mPeripheralInstance = nullptr;

PowerSource::PowerSource()
{
	mPeripheralInstance = this;
}

void PowerSource::initialize()
{

}

void PowerSource::shutdown()
{

}
