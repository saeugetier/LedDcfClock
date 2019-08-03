/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rtc.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PUSHBUTTON_Pin LL_GPIO_PIN_0
#define PUSHBUTTON_GPIO_Port GPIOA
#define SETTINGS2_Pin LL_GPIO_PIN_2
#define SETTINGS2_GPIO_Port GPIOA
#define SETTINGS1_Pin LL_GPIO_PIN_4
#define SETTINGS1_GPIO_Port GPIOA
#define TESTPOINT_Pin LL_GPIO_PIN_5
#define TESTPOINT_GPIO_Port GPIOA
#define WS2812_Pin LL_GPIO_PIN_0
#define WS2812_GPIO_Port GPIOB
#define POWER_SOURCE_Pin LL_GPIO_PIN_1
#define POWER_SOURCE_GPIO_Port GPIOB
#define POWER_MODE_Pin LL_GPIO_PIN_8
#define POWER_MODE_GPIO_Port GPIOA
#define LED1_Pin LL_GPIO_PIN_3
#define LED1_GPIO_Port GPIOB
#define PON_IN_Pin LL_GPIO_PIN_4
#define PON_IN_GPIO_Port GPIOB
#define DCF_IN_Pin LL_GPIO_PIN_5
#define DCF_IN_GPIO_Port GPIOB
#define DCF_IN_TIM_Pin LL_GPIO_PIN_6
#define DCF_IN_TIM_GPIO_Port GPIOB
#define LED2_Pin LL_GPIO_PIN_8
#define LED2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
