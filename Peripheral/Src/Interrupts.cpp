/*
 * Interrupts.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_system.h"

#include "PulseDetector.h"
#include "DcfPowerdown.h"
#include "DcfWakeup.h"
#include "SystemTick.h"
#include "Buttons.h"
#include "PowerSource.h"
#include "Clock.h"

//using extern "C" to get the right function reference -- ABI changed for C++ function calls
extern "C"
{
void TIM1_CC_IRQHandler(void)
{
	if(PulseDetector::getInstance() != nullptr)
		PulseDetector::getInstance()->handleInterrupt();
}
}

extern "C"
{
void SysTick_Handler()
{
	if(SystemTick::getInstance() != nullptr)
		SystemTick::getInstance()->handleInterrupt();
}
}

extern "C"
{
void EXTI0_1_IRQHandler()
{
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_0))
	{
		if(PushButton::getInstance() != nullptr)
			PushButton::getInstance()->handleInterrupt();
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_0);
	}
	if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_1) || LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_1))
	{
		if(PowerSource::getInstance() != nullptr)
			PowerSource::getInstance()->handleInterrupt();

		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_1);
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_1);
	}
}
}

extern "C"
{
void EXTI2_3_IRQHandler()
{
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_2))
	{
		if(Settings2Button::getInstance() != nullptr)
			Settings2Button::getInstance()->handleInterrupt();
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_2);
	}
}
}

extern "C"
{
void EXTI4_15_IRQHandler()
{
	// @TODO: EXTI_LINE_4 is used double. Proper handling has to be implemented

	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_4)) //Buttons react on falling edges
	{
		if(Settings1Button::getInstance() != nullptr)
			Settings1Button::getInstance()->handleInterrupt();
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_4);
	}
	if(LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_4)) //Dcf Powerdown reacts on rising edges
	{
		if(DcfPowerdown::getInstance() != nullptr)
			DcfPowerdown::getInstance()->handleInterrupt();
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_4);
	}
	else if(LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_5))
	{
		if(DcfWakeup::getInstance() != nullptr)
			DcfWakeup::getInstance()->handleInterrupt();
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_5);
	}
}
}

extern "C"
{
void RTC_TAMP_IRQHandler()
{
	if(RtcClock::getInstance() != nullptr)
		RtcClock::getInstance()->handleInterrupt();
}
}

