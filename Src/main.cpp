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
#ifdef DEBUG
#include "SEGGER_RTT.h"
#endif

extern "C"
{
	void SystemClock_Config(void);
}




int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Configure the system clock */
  SystemClock_Config();

  /** Enable the PVD Output  */
  //LL_PWR_EnablePVD();

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
  //MX_TIM1_Init();

#ifdef DEBUG
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
  LL_DBGMCU_EnableDBGStopMode();
  LL_DBGMCU_EnableDBGStandbyMode();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_DBGMCU);
#endif

  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);

  SystemManager manager;

#ifdef DEBUG
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

  SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");
  SEGGER_RTT_WriteString(0, "###### Testing SEGGER_printf() ######\r\n");
#endif

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    manager.runTasks();
  }
}
