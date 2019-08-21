/*
 * WS2812.h
 *
 *  Created on: May 30, 2019
 *      Author: timm
 */

#ifndef PERIPHERAL_INC_WS2812_H_
#define PERIPHERAL_INC_WS2812_H_

#include "Peripheral.h"
#include <cstring>
#include "dma.h"
#include "tim.h"

const uint8_t gamma[] = {
	0,  1,  1,  1,  1,  1,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  4,  4,  5,  5,  5,
    6,  7,  7,  8,  9,  9, 10, 11, 12, 13,
   14, 16, 17, 19, 21, 22, 24, 27, 29, 32,
   35, 38, 41, 45, 49, 54, 58, 64, 70, 76,
   83, 90, 99,107,117,128,139,152,166,181,
   197,215,235,255
};

struct RGB
{
	RGB() : r(0), g(0), b(0) {}
	RGB(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

template<int NumLeds>
class WS2812 : public Peripheral
{
public:
	WS2812() : mDmaBuffer{0}
    {
	}

	void clearBuffer()
	{
		memset(mPixelColor, 0, sizeof(mPixelColor));
	}

	void setPixelColor(RGB color, int index)
	{
		mPixelColor[index] = color;
	}

	RGB getPixelColor(int index)
	{
		return mPixelColor[index];
	}

	void showLeds()
	{
		updateLedBuffer();
		startDma();
	}

	bool isBusy()
	{
		return !LL_DMA_IsActiveFlag_TC1(DMA1) && LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1);
	}


protected:
	virtual void initialize()
	{
		DMA_Init();
		TIM3_Init();

		memset(mDmaBuffer, 0, sizeof(mDmaBuffer));

		clearBuffer();
	}

	virtual void shutdown()
	{
		DMA_DeInit();
		TIM3_DeInit();
	}

	RGB mPixelColor[NumLeds];
	uint32_t mDmaBuffer[NumLeds*24 + 25];

	void DMA_Init()
	{
		/* Init with LL driver */
		/* DMA controller clock enable */
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

		/* DMA interrupt init */
		/* DMA1_Channel1_IRQn interrupt configuration */
		NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
		NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	}

	void DMA_DeInit()
	{
		NVIC_DisableIRQ(DMA1_Channel1_IRQn);
		LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	}

	void TIM3_Init()
	{
		LL_TIM_InitTypeDef TIM_InitStruct = {0};
		LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

		LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
		/* Peripheral clock enable */
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

		/* TIM3 DMA Init */

		/* TIM3_UP Init */
		LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_TIM3_UP);

		LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

		LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);

		LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);

		LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

		LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

		LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_WORD);

		LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_WORD);

		TIM_InitStruct.Prescaler = 1;
		TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
		TIM_InitStruct.Autoreload = 10;
		TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
		LL_TIM_Init(TIM3, &TIM_InitStruct);
		LL_TIM_DisableARRPreload(TIM3);
		LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
		TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
		TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
		TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
		TIM_OC_InitStruct.CompareValue = 0;
		TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
		LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct);
		LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH3);
		LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
		LL_TIM_DisableMasterSlaveMode(TIM3);

		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
		/**TIM3 GPIO Configuration
		PB0     ------> TIM3_CH3
		*/
		GPIO_InitStruct.Pin = WS2812_Pin;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
		LL_GPIO_Init(WS2812_GPIO_Port, &GPIO_InitStruct);

		LL_TIM_OC_SetCompareCH1(TIM3, 0);
		LL_TIM_EnableCounter(TIM3);
		for(int i = 0; i < 3000; i++);
		LL_TIM_DisableCounter(TIM3);

		LL_TIM_ClearFlag_UPDATE(TIM3);

		LL_TIM_ConfigDMABurst(TIM3, LL_TIM_DMABURST_BASEADDR_CCR3, LL_TIM_DMABURST_LENGTH_1TRANSFER);
		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
		LL_TIM_EnableAllOutputs(TIM3);
		LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
		LL_TIM_EnableCounter(TIM3);
		LL_TIM_EnableDMAReq_UPDATE(TIM3);
	}

	void TIM3_DeInit()
	{
		LL_GPIO_SetPinMode(WS2812_GPIO_Port, WS2812_Pin, LL_GPIO_MODE_ANALOG);
		LL_TIM_DeInit(TIM3);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);
	}

	void updateLedBuffer()
	{
		mDmaBuffer[0] = 0; //one extra bit in front for synchronization
		int dmaBufferPos = 1;
		for(int i = 0; i < NumLeds; i++)
		{
			RGB color = mPixelColor[i];
			color.r = gamma[color.r / 4];
			color.g = gamma[color.g / 4];
			color.b = gamma[color.b / 4];
			for(int j = 0; j < 8; j++)
			{
				mDmaBuffer[dmaBufferPos + j] = color.g & 0x80 ? 0x06 : 0x03;
				mDmaBuffer[dmaBufferPos + j + 8] = color.r & 0x80 ? 0x06 : 0x03;
				mDmaBuffer[dmaBufferPos + j + 16] = color.b & 0x80 ? 0x06 : 0x03;
				color.r = color.r << 1;
				color.g = color.g << 1;
				color.b = color.b << 1;
			}
			dmaBufferPos += 24;
		}
	}

	void startDma()
	{
		LL_DMA_ClearFlag_TC1(DMA1);

		LL_TIM_DisableCounter(TIM3);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&mDmaBuffer, (uint32_t)&TIM3->DMAR, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, sizeof(mDmaBuffer)/sizeof(uint32_t));

		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_TIM_EnableCounter(TIM3);
	}
};


#endif /* PERIPHERAL_INC_WS2812_H_ */
