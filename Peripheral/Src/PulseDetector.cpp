#include "PulseDetector.h"
#include "main.h"
#include <cstddef>

PulseDetector::PulseDetector(bool SyncOnRisingFlank) : mPulseCallback(0)
{
	mSyncOnRisingFlank = SyncOnRisingFlank;
}

void PulseDetector::initialize()
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	/**TIM1 GPIO Configuration
	PB6   ------> TIM1_CH3
	*/
	GPIO_InitStruct.Pin = DCF_IN_TIM_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(DCF_IN_TIM_GPIO_Port, &GPIO_InitStruct);

	/* TIM1 interrupt Init */
	NVIC_SetPriority(TIM1_CC_IRQn, 0);
	NVIC_EnableIRQ(TIM1_CC_IRQn);

	TIM_InitStruct.Prescaler = 16000;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0xFFFF;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM1, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM1);
	LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
	LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM1);
	LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetPrescaler(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetFilter(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_IC_FILTER_FDIV1_N4);
	LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_IC_POLARITY_RISING);
	LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_ACTIVEINPUT_INDIRECTTI);
	LL_TIM_IC_SetPrescaler(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetFilter(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_IC_POLARITY_FALLING);
	TIM_BDTRInitStruct.BreakAFMode = LL_TIM_BREAK_AFMODE_INPUT;
	TIM_BDTRInitStruct.Break2AFMode = LL_TIM_BREAK_AFMODE_INPUT;
	LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);
}

void PulseDetector::shutdown()
{
	LL_TIM_DisableCounter(TIM1);
	LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);
}

void PulseDetector::registerPulseCallback(Callback* callback)
{
	mPulseCallback = callback;
}

void PulseDetector::registerWakeupCallback(Callback* callback)
{
	mWakeupCallback = callback;
}

void PulseDetector::handleInterrupt()
{
	LL_TIM_ClearFlag_UPDATE(TIM1);

	LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

	if(LL_TIM_IsActiveFlag_CC3(TIM1))
	{
		mEdgeHigh = LL_TIM_IC_GetCaptureCH3(TIM1);
		if(!mSyncOnRisingFlank)
		{
			LL_TIM_SetCounter(TIM1, 0);
			if(mPulseCallback)
				mPulseCallback->notify();
		}
	}

	if(LL_TIM_IsActiveFlag_CC4(TIM1))
	{
		mEdgeLow = LL_TIM_IC_GetCaptureCH4(TIM1);
		if(mSyncOnRisingFlank)
		{
			LL_TIM_SetCounter(TIM1, 0);
			if(mPulseCallback)
				mPulseCallback->notify();
		}
	}

	LL_TIM_ClearFlag_CC3(TIM1);

	LL_TIM_ClearFlag_CC4(TIM1);

	LL_TIM_ClearFlag_CC3OVR(TIM1);

	LL_TIM_ClearFlag_CC4OVR(TIM1);
}

uint32_t PulseDetector::getLowEdge()
{
	return mEdgeLow;
}

uint32_t PulseDetector::getHighEdge()
{
	return mEdgeHigh;
}


