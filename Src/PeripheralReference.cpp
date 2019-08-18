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
#include "WS2812.h"
#include "LedPowerEnable.h"
#include "PowerSource.h"
#include "TestPoint.h"
#include "SupplyVoltageSupervisor.h"
#include "SettingsStorage.h"

template class PeripheralReference<SystemTick> ;
template class PeripheralReference<PulseDetector> ;
template class PeripheralReference<DcfPowerdown>;
template class PeripheralReference<DcfWakeup> ;
template class PeripheralReference<StatusLed1> ;
template class PeripheralReference<StatusLed2> ;
template class PeripheralReference<RtcClock> ;
template class PeripheralReference<PushButton> ;
template class PeripheralReference<Settings1Button> ;
template class PeripheralReference<Settings2Button> ;
template class PeripheralReference<WS2812<60+12>> ;
template class PeripheralReference<LedPowerEnable> ;
template class PeripheralReference<PowerSource> ;
template class PeripheralReference<TestPoint> ;
template class PeripheralReference<SupplyVoltageSupervisor> ;
template class PeripheralReference<SettingsStorage> ;

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
periph& PeripheralReference<periph>::getInstance() const
{
	return mReference;
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
