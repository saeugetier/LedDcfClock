/*
 * Buttons.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: timm
 */

#include "Buttons.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_gpio.h"
#include "main.h"

template<>
PushButton* InterruptPeripheral<PushButton>::mPeripheralInstance = nullptr;

template<>
Settings1Button* InterruptPeripheral<Settings1Button>::mPeripheralInstance = nullptr;

template<>
Settings2Button* InterruptPeripheral<Settings2Button>::mPeripheralInstance = nullptr;

PushButton::PushButton()
{
	mPeripheralInstance = this;
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
	LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN1);
}

void PushButton::clearWakeupFlag()
{
	LL_PWR_ClearFlag_WU1();
}

void PushButton::handleInterrupt()
{
	mCallback->notify();
}

bool PushButton::isWakeupFlagSet()
{
	return LL_PWR_IsActiveFlag_WU1();
}

bool PushButton::getButtonPressed()
{
	return !LL_GPIO_IsInputPinSet(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin);
}

void PushButton::initialize()
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE0);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE0);

	NVIC_EnableIRQ (EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
}

void PushButton::shutdown()
{
	LL_GPIO_SetPinMode(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_MODE_ANALOG);
}


Settings1Button::Settings1Button()
{
	mPeripheralInstance = this;
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
	LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN2);
}

void Settings1Button::clearWakeupFlag()
{
	LL_PWR_ClearFlag_WU1();
}

void Settings1Button::handleInterrupt()
{
	mCallback->notify();
}

bool Settings1Button::isWakeupFlagSet()
{
	return LL_PWR_IsActiveFlag_WU2();
}

bool Settings1Button::getButtonPressed()
{
	return !LL_GPIO_IsInputPinSet(SETTINGS1_GPIO_Port, SETTINGS1_Pin);
}

void Settings1Button::initialize()
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE4);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE4);

	NVIC_EnableIRQ (EXTI4_15_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
}

void Settings1Button::shutdown()
{
	LL_GPIO_SetPinMode(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_MODE_ANALOG);
}

Settings2Button::Settings2Button()
{
	mPeripheralInstance = this;
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);
	LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN4);

}

void Settings2Button::clearWakeupFlag()
{
	LL_PWR_ClearFlag_WU1();
}

void Settings2Button::handleInterrupt()
{
	mCallback->notify();
}

bool Settings2Button::isWakeupFlagSet()
{
	return LL_PWR_IsActiveFlag_WU4();
}

bool Settings2Button::getButtonPressed()
{
	return !LL_GPIO_IsInputPinSet(SETTINGS2_GPIO_Port, SETTINGS2_Pin);
}

void Settings2Button::initialize()
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE2);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE2);

	NVIC_EnableIRQ (EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn, 0);
}

void Settings2Button::shutdown()
{
	LL_GPIO_SetPinMode(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_MODE_ANALOG);
}
