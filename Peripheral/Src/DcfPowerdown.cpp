/*
 * DcfPowerdown.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "DcfPowerdown.h"

template<>
DcfPowerdown* InterruptPeripheral<DcfPowerdown>::mPeripheralInstance = nullptr;

DcfPowerdown::DcfPowerdown()
{
	mPeripheralInstance = this;
}

void DcfPowerdown::initialize()
{

}

void DcfPowerdown::shutdown()
{

}

void DcfPowerdown::handleInterrupt()
{

}
