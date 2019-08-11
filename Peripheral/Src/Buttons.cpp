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

Button::Button(ButtonPin pin)
{
	mPin = pin;

	switch(pin)
	{
	case PUSH_BUTTON_PIN:
		LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
		LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN1);
		break;
	case SETTINGS1_BUTTON_PIN:
		LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
		LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN2);
		break;
	case SETTINGS2_BUTTON_PIN:
		LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);
		LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN4);
		break;
	}
}

void Button::initialize()
{
	setPinToInterrupt();
}

void Button::shutdown()
{
	disableInterrupt();
}

void Button::disableInterrupt()
{

}

void Button::setPinToInterrupt()
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

	/* GPIO Port Clock Enable */
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	switch(mPin)
	{
	case PUSH_BUTTON_PIN:
		LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE0);
		EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
		EXTI_InitStruct.LineCommand = ENABLE;
		EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
		EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
		LL_EXTI_Init(&EXTI_InitStruct);
		LL_GPIO_SetPinPull(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_PULL_NO);
		LL_GPIO_SetPinMode(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_MODE_INPUT);
		break;
	case SETTINGS1_BUTTON_PIN:
		LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE4);
		EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
		EXTI_InitStruct.LineCommand = ENABLE;
		EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
		EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
		LL_EXTI_Init(&EXTI_InitStruct);
		LL_GPIO_SetPinPull(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_PULL_NO);
		LL_GPIO_SetPinMode(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_MODE_INPUT);
		break;
	case SETTINGS2_BUTTON_PIN:
		LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE2);
		EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
		EXTI_InitStruct.LineCommand = ENABLE;
		EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
		EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
		LL_EXTI_Init(&EXTI_InitStruct);
		LL_GPIO_SetPinPull(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_PULL_NO);
		LL_GPIO_SetPinMode(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_MODE_INPUT);
		break;
	}
}

bool Button::getButtonPressed()
{
	return false;
}

bool Button::isWakeupFlagSet()
{
	bool result = false;
	switch(mPin)
	{
	case PUSH_BUTTON_PIN:
		result = LL_PWR_IsActiveFlag_WU1();
		break;
	case SETTINGS1_BUTTON_PIN:
		result = LL_PWR_IsActiveFlag_WU2();
		break;
	case SETTINGS2_BUTTON_PIN:
		result = LL_PWR_IsActiveFlag_WU4();
		break;
	default:
		break;
	}

	return result;
}

void Button::clearWakeupFlag()
{
	switch(mPin)
	{
	case PUSH_BUTTON_PIN:
		LL_PWR_ClearFlag_WU1();
		break;
	case SETTINGS1_BUTTON_PIN:
		LL_PWR_ClearFlag_WU2();
		break;
	case SETTINGS2_BUTTON_PIN:
		LL_PWR_ClearFlag_WU4();
		break;
	}
}

void Button::handleInterrupt()
{

}


