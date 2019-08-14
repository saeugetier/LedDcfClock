/*
 * StatusLeds.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: timm
 */

#include <StatusLed.h>
#include "main.h"

void StatusLed1::initialize()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);

	GPIO_InitStruct.Pin = LED1_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);
}

void StatusLed1::shutdown()
{
	LL_GPIO_SetPinMode(LED1_GPIO_Port, LED1_Pin, LL_GPIO_MODE_ANALOG);
}

void StatusLed1::setLed(bool on)
{
	if(on)
		LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
	else
		LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

void StatusLed2::initialize()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);

	GPIO_InitStruct.Pin = LED2_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);
}

void StatusLed2::shutdown()
{
	LL_GPIO_SetPinMode(LED2_GPIO_Port, LED2_Pin, LL_GPIO_MODE_ANALOG);
}

void StatusLed2::setLed(bool on)
{
	if(on)
		LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED1_Pin);
	else
		LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED1_Pin);
}
