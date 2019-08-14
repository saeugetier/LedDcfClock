/*
 * DcfWakeup.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: timm
 */

#include "DcfWakeup.h"
#include "stm32g0xx_ll_pwr.h"
#include "main.h"

template<>
DcfWakeup* InterruptPeripheral<DcfWakeup>::mPeripheralInstance = nullptr;

DcfWakeup::DcfWakeup()
{
	mPeripheralInstance = this;
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN6);
}

void DcfWakeup::initialize()
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE5);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(DCF_IN_GPIO_Port, DCF_IN_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(DCF_IN_GPIO_Port, DCF_IN_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE5);

	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_5);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_5);

	NVIC_EnableIRQ (EXTI4_15_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
}

void DcfWakeup::shutdown()
{

}

void DcfWakeup::handleInterrupt()
{
	if (mCallback != nullptr)
		mCallback->notify();
}
