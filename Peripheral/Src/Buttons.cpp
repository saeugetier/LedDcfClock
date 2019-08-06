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

void Buttons::initialize()
{
	setPinsToInterrupt();
}

void Buttons::shutdown()
{
	setPinsToWakeup();
}

void Buttons::setPinsToWakeup()
{
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);
}

void Buttons::setPinsToInterrupt()
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

	/* Disable Wakeup before using the interrupts */
	LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);
	LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);
	LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN4);

	/* GPIO Port Clock Enable */
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE0);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE4);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(SETTINGS1_GPIO_Port, SETTINGS1_Pin, LL_GPIO_MODE_INPUT);

	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE2);
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_GPIO_SetPinPull(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinMode(SETTINGS2_GPIO_Port, SETTINGS2_Pin, LL_GPIO_MODE_INPUT);
}

Buttons::ButtonSource Buttons::getButtonsPressed()
{
	Buttons::ButtonSource source = NONE;
	source = source | LL_GPIO_IsInputPinSet(PUSHBUTTON_GPIO_Port, PUSHBUTTON_Pin) ? PUSHBUTTON : NONE;
	source = source | LL_GPIO_IsInputPinSet(SETTINGS1_GPIO_Port, SETTINGS1_Pin) ? SETTINGSBUTTON1 : NONE;
	source = source | LL_GPIO_IsInputPinSet(SETTINGS2_GPIO_Port, SETTINGS2_Pin) ? SETTINGSBUTTON2 : NONE;
	return source;
}

bool Buttons::isWakeupFlagSet()
{
	if(LL_PWR_IsActiveFlag_WU1() || LL_PWR_IsActiveFlag_WU2() || LL_PWR_IsActiveFlag_WU4())
		return true;
	return false;
}

void Buttons::clearWakeupFlags()
{
	LL_PWR_ClearFlag_WU1();
	LL_PWR_ClearFlag_WU2();
	LL_PWR_ClearFlag_WU4();
}

void Buttons::handleInterrupt()
{

}


