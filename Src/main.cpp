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
#include "SystemManager.h"

extern "C"
{
	void SystemClock_Config(void);
}


SystemManager manager;

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /** Enable the PVD Output  */
  LL_PWR_EnablePVD();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();

#ifdef DEBUG
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
  LL_DBGMCU_EnableDBGStopMode();
  LL_DBGMCU_EnableDBGStandbyMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
#endif

  PWR->PDCRA = PWR_PDCRA_PD8;
  PWR->CR3 |= PWR_CR3_APC | PWR_CR3_ENB_ULP;
  LL_GPIO_SetOutputPin(POWER_MODE_GPIO_Port, POWER_MODE_Pin);

  LL_SYSTICK_EnableIT();
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);

  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_BKP_SetRegister(TAMP, 0, 0x66);
  LL_RTC_EnableWriteProtection(RTC);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    manager.runTasks();
    manager.getSystemTickReference().init();
  }
}
