/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <main.h>
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LedClock.h"
#include "Dcf77.h"

extern "C"
{
	void SystemClock_Config(void);
}

time_t current_time;
volatile uint32_t systickcounter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t clock_state;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

Settings settings;

LedClock ledClock(&settings);
RtcClock _clock;
DCF77 dcf(&_clock, false);

void EnterStandbyMode(void)
{
  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN4);

  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU();

  /* Set wakeup pin polarity */
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN2);
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN4);

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);

  /* As default User push-button state is high level, need to clear all wake up Flag again */
  LL_PWR_ClearFlag_WU();

  /** Request to enter Standby mode
    * Following procedure describe in STM32G0xx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Set Standby mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();

  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

int second = 0xFF;

time_t dcf_time = 0;

int main(void)
{
  /* USER CODE BEGIN 1 */
  struct tm y2k = {0};

  //ws2812.initialize();

  y2k.tm_hour = 2;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 102; y2k.tm_mon = 3; y2k.tm_mday = 1;

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /** Enable the PVD Output 
  */
  LL_PWR_EnablePVD();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_RTC_Init();
  _clock.initialize();
  MX_TIM1_Init();

  ledClock.enable(true);

  /* USER CODE BEGIN 2 */
#ifdef DEBUG
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
  LL_DBGMCU_EnableDBGStopMode();
  LL_DBGMCU_EnableDBGStandbyMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
#endif
  //DCF77_Clock::setup();
  //DCF77_Clock::set_input_provider(sample_input_pin);

  current_time = mktime(&y2k);

  PWR->PDCRA = PWR_PDCRA_PD8;
  PWR->CR3 |= PWR_CR3_APC | PWR_CR3_ENB_ULP;
  LL_GPIO_SetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);

  //for(int i = 0; i < 32000; i++);

  LL_TIM_ClearFlag_UPDATE(TIM3);

  LL_TIM_ConfigDMABurst(TIM3, LL_TIM_DMABURST_BASEADDR_CCR3, LL_TIM_DMABURST_LENGTH_1TRANSFER);
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_EnableAllOutputs(TIM3);
  LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
  LL_TIM_EnableCounter(TIM3);
  LL_TIM_EnableDMAReq_UPDATE(TIM3);

  LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_ACTIVEINPUT_INDIRECTTI);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
  LL_TIM_EnableIT_CC3(TIM1);
  LL_TIM_EnableIT_CC4(TIM1);
  LL_TIM_EnableCounter(TIM1);

  //LL_GPIO_ResetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);

  //if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  //  {
      /* Starting PWM generation Error */
   //   Error_Handler();
   // }

    /*## Start DMA Burst transfer ###########################################*/
   // HAL_TIM_DMABurst_WriteStart(&htim2, TIM_DMABASE_ARR, TIM_DMA_UPDATE,
    //                            (uint32_t*)aSRC_Buffer, TIM_DMABURSTLENGTH_3TRANSFERS);

  LL_SYSTICK_EnableIT();
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
  /* USER CODE END 2 */

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_BKP_SetRegister(TAMP, 0, 0x66);
  LL_RTC_EnableWriteProtection(RTC);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 /*int length = dcf77_encode(&current_time);
	 if(length == 0)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(100);
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(900);
	 }
	 else if(length == 1)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(200);
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
		 delay_ms(800);
	 }
	 else
	 {
		 delay_ms(1000);
	 }

	 current_time++;*/

	 //clock_state = DCF77_Clock::get_clock_state();
	 /*if(clock_state != 0)
	 {
		 LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
	 }
	 else
	 {
		 LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);
	 }*/

	  //__WFI();

	  //if(LL_RTC_TIME_GetSubSecond(RTC) < 20)
	  //{
		  //LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

		  if(LL_RTC_IsActiveFlag_RS(RTC))
		  {

			  //LL_RTC_TIME_Get(RTC);
			  LL_RTC_DATE_Get(RTC);

			  y2k.tm_hour = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
			  y2k.tm_min = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
			  y2k.tm_sec =  __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));

			  ledClock.displayTime(y2k, true, (LL_RTC_TIME_GetSubSecond(RTC) * 128) / 1025);

				  //ws2812.clearBuffer();

				  /*second = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
				  int subsecond = (LL_RTC_TIME_GetSubSecond(RTC) * 200) / 1025;
				  int minute = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
				  int hour = (__LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)) % 12) * 5;

				  ws2812.setPixelColor(RGB(0,0,subsecond), second);
				  ws2812.setPixelColor(RGB(0,0,200-subsecond), (second + 1) % 60);
				  ws2812.setPixelColor(RGB(200,0,ws2812.getPixelColor(minute).b), minute);
				  ws2812.setPixelColor(RGB(ws2812.getPixelColor(hour).r,200,ws2812.getPixelColor(hour).b), hour);
				  ws2812.showLeds();*/

				  /*if(!(second % 40) && second != 0)
				  {
					  EnterStandbyMode();
				  }*/

			  LL_RTC_ClearFlag_RS(RTC);

		  }
		  while(!ledClock.isReady());

		  int temp_time = dcf.getTime();
		  if(temp_time != 0)
			  dcf_time = temp_time;

	 //}
	 // else
	  //{
		//  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
	  //}
  }
  /* USER CODE END 3 */
}
