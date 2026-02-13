/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TOGGLE5_Pin GPIO_PIN_4
#define TOGGLE5_GPIO_Port GPIOA
#define REGPG5_Pin GPIO_PIN_5
#define REGPG5_GPIO_Port GPIOA
#define REGPG6_Pin GPIO_PIN_6
#define REGPG6_GPIO_Port GPIOA
#define TOGGLE6_Pin GPIO_PIN_7
#define TOGGLE6_GPIO_Port GPIOA
#define TOGGLE7_Pin GPIO_PIN_0
#define TOGGLE7_GPIO_Port GPIOB
#define REGPG7_Pin GPIO_PIN_1
#define REGPG7_GPIO_Port GPIOB
#define REGPG4_Pin GPIO_PIN_10
#define REGPG4_GPIO_Port GPIOB
#define TOGGLE4_Pin GPIO_PIN_11
#define TOGGLE4_GPIO_Port GPIOB
#define REGPG3_Pin GPIO_PIN_13
#define REGPG3_GPIO_Port GPIOB
#define TOGGLE3_Pin GPIO_PIN_14
#define TOGGLE3_GPIO_Port GPIOB
#define REGPG2_Pin GPIO_PIN_15
#define REGPG2_GPIO_Port GPIOB
#define TOGGLE2_Pin GPIO_PIN_8
#define TOGGLE2_GPIO_Port GPIOA
#define REGPG1_Pin GPIO_PIN_9
#define REGPG1_GPIO_Port GPIOA
#define TOGGLE1_Pin GPIO_PIN_10
#define TOGGLE1_GPIO_Port GPIOA
#define PWM1_Pin GPIO_PIN_15
#define PWM1_GPIO_Port GPIOA
#define PWM2_Pin GPIO_PIN_3
#define PWM2_GPIO_Port GPIOB
#define PWM3_Pin GPIO_PIN_4
#define PWM3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
