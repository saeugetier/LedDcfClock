#include "LedPowerEnable.h"
#include "main.h"

void LedPowerEnable::initialize()
{
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = POWER_MODE_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(POWER_MODE_GPIO_Port, &GPIO_InitStruct);
}

void LedPowerEnable::shutdown()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = POWER_MODE_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(POWER_MODE_GPIO_Port, &GPIO_InitStruct);
}

void LedPowerEnable::setPower(bool on)
{
	if(on)
		LL_GPIO_SetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);
	else
		LL_GPIO_ResetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);
}
