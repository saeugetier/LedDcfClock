/*
 * PeripheralReference.cpp
 *
 *  Created on: 08.08.2019
 *      Author: timm
 */

#include "PeripheralReference.h"
#include "SystemTick.h"
#include "PulseDetector.h"
#include "Clock.h"
#include "DcfPowerdown.h"
#include "DcfWakeup.h"
#include "Buttons.h"
#include "StatusLed.h"

template class PeripheralReference<SystemTick> ;
template class PeripheralReference<PulseDetector> ;
template class PeripheralReference<DcfPowerdown>;
template class PeripheralReference<DcfWakeup> ;
template class PeripheralReference<StatusLed> ;
template class PeripheralReference<RtcClock> ;
template class PeripheralReference<PushButton> ;

template<class periph>
uint8_t PeripheralReference<periph>::mInitializationCounter = 0;

template<class periph>
PeripheralReference<periph>::PeripheralReference(periph& peripheral) :
		mInitializationRequested(false), mReference(peripheral)
{
}

template<class periph>
PeripheralReference<periph>::PeripheralReference(const periph& peripheral) :
		mInitializationRequested(false), mReference(const_cast<periph&>(peripheral))
{
}

template<class periph>
PeripheralReference<periph>::PeripheralReference(const PeripheralReference& copy) :
		mInitializationRequested(false), mReference(copy.mReference)
{
}

template<class periph>
void PeripheralReference<periph>::init()
{
	if (!mInitializationRequested)
	{
		if (mInitializationCounter == 0)
			_init(mReference);
		mInitializationCounter++;
	}
	mInitializationRequested = true;
}

template<class periph>
void PeripheralReference<periph>::shutdown()
{
	if (mInitializationRequested)
	{
		mInitializationCounter--;
		if (mInitializationCounter == 0)
			_shutdown(mReference);
	}
	mInitializationRequested = false;
}

template<class periph>
PeripheralReference<periph>::~PeripheralReference()
{
	shutdown();
}
