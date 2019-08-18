/*
 * PowerSource.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#include "PowerSource.h"
#include "main.h"

template<>
PowerSource* InterruptPeripheral<PowerSource>::mPeripheralInstance = nullptr;

PowerSource::PowerSource()
{
	mPeripheralInstance = this;
}

void PowerSource::initialize()
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE1);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(POWER_SOURCE_GPIO_Port, POWER_SOURCE_Pin, LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinMode(POWER_SOURCE_GPIO_Port, POWER_SOURCE_Pin, LL_GPIO_MODE_INPUT);

	NVIC_EnableIRQ (EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
}

void PowerSource::shutdown()
{
	LL_GPIO_SetPinMode(POWER_SOURCE_GPIO_Port, POWER_SOURCE_Pin, LL_GPIO_MODE_ANALOG);
}

void PowerSource::handleInterrupt()
{
	if(mCallback != nullptr)
		mCallback->notify();
}

PowerSource::Source PowerSource::getSource()
{
	uint32_t pinSet = LL_GPIO_IsInputPinSet(POWER_SOURCE_GPIO_Port, POWER_SOURCE_Pin);
	return pinSet ? Source::USB : Source::BATTERY;
}
