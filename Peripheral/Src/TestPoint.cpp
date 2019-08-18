/*
 * TestPoint.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: timm
 */

#include "TestPoint.h"
#include "main.h"

TestPoint::TestPoint()
{

}

void TestPoint::shutdown()
{
	LL_GPIO_SetPinMode(TESTPOINT_GPIO_Port, TESTPOINT_Pin, LL_GPIO_MODE_ANALOG);
}

void TestPoint::setOutput(bool high)
{
	if(high)
		LL_GPIO_SetOutputPin(TESTPOINT_GPIO_Port, TESTPOINT_Pin);
	else
		LL_GPIO_ResetOutputPin(TESTPOINT_GPIO_Port, TESTPOINT_Pin);
}

void TestPoint::initialize()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetOutputPin(TESTPOINT_GPIO_Port, TESTPOINT_Pin);

	GPIO_InitStruct.Pin = TESTPOINT_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(TESTPOINT_GPIO_Port, &GPIO_InitStruct);
}
