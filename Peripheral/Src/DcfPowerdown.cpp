/*
 * DcfPowerdown.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "DcfPowerdown.h"
#include "main.h"

template<>
DcfPowerdown* InterruptPeripheral<DcfPowerdown>::mPeripheralInstance = nullptr;

DcfPowerdown::DcfPowerdown()
{
	mPeripheralInstance = this;
}

void DcfPowerdown::initialize()
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE4);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(PON_IN_GPIO_Port, PON_IN_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(PON_IN_GPIO_Port, PON_IN_Pin, LL_GPIO_MODE_INPUT);

	NVIC_EnableIRQ (EXTI4_15_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
}

void DcfPowerdown::shutdown()
{
	LL_GPIO_SetPinMode(PON_IN_GPIO_Port, PON_IN_Pin, LL_GPIO_MODE_ANALOG);
}

void DcfPowerdown::handleInterrupt()
{
	if(mCallback != nullptr)
		mCallback->notify();
}
