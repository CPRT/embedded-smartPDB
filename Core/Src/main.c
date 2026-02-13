/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SERVO1_HIGH 0x00
#define SERVO1_LOW  0x01
#define SERVO2_HIGH 0x02
#define SERVO2_LOW  0x03
#define SERVO3_HIGH 0x04
#define SERVO3_LOW  0x05
#define SERVO4_HIGH 0x06
#define SERVO4_LOW  0x07
#define TOGGLE      0x08
#define REGPG       0x09

#define MAX_TRANSFER 10
#define NUM_REGISTERS 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

uint8_t REGISTERS[NUM_REGISTERS] = {
  0x05, // SERVO1_HIGH
  0xDC, // SERVO1_LOW
  0x05, // SERVO2_HIGH
  0xDC, // SERVO2_LOW
  0x05, // SERVO3_HIGH
  0xDC, // SERVO3_LOW
  0x05, // SERVO4_HIGH
  0xDC, // SERVO4_LOW
  0x00, // TOGGLE
  0x00  // REGPG
};

uint8_t RxData[MAX_TRANSFER];
uint8_t RxLen = 0;
uint8_t TxLen = 0;
uint8_t TxReg = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

void WriteRegs();
void WriteServos();
void WritePins();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  if(TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
  {
	RxData[0] = 0;
	RxLen = 0;
	TxLen = 0;
    HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData, 1, I2C_FIRST_FRAME);
  }else {
	TxLen = 0;
	TxReg = RxData[0];
	RxData[0] = 0;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, REGISTERS + TxReg, 1, I2C_FIRST_FRAME);
  }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  RxLen++;
  if (RxLen < MAX_TRANSFER)
  {
	if (RxLen == MAX_TRANSFER-1)
	{
	  HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData + RxLen, 1, I2C_LAST_FRAME);
	}else {
      HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData + RxLen, 1, I2C_NEXT_FRAME);
	}
  }

  if (RxLen == MAX_TRANSFER)
  {
	WriteRegs();
	HAL_I2C_EnableListen_IT(hi2c);
  }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  TxLen++;
  HAL_I2C_Slave_Seq_Transmit_IT(hi2c, REGISTERS + TxReg + TxLen, 1, I2C_NEXT_FRAME);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  uint32_t errorcode = HAL_I2C_GetError(hi2c);

  if (errorcode & HAL_I2C_ERROR_AF) // Normal behaviour, transaction was less than MAX_TRANSFER bytes
  {
	if (TxLen == 0) {
	  WriteRegs();
	  RxLen = 0;
	}else {
	  TxLen = 0;
	}
  }
  if (errorcode & HAL_I2C_ERROR_BERR)
  {
  	HAL_I2C_DeInit(hi2c);
  	HAL_I2C_Init(hi2c);
  	memset(RxData,'\0',MAX_TRANSFER);
  	RxLen = 0;
  	TxLen = 0;
  }

  HAL_I2C_EnableListen_IT(hi2c);
}

void WriteRegs()
{
  uint8_t reg = RxData[0];

  if (reg + RxLen - 1 > NUM_REGISTERS) {
	return; // Ignore writes beyond the end of address space
  }

  for (int i = 0; i < RxLen - 1; i++)
  {
	REGISTERS[reg++] = RxData[i + 1];
  }

  WriteServos();
  WritePins();
}

void WriteServos() {
  TIM2->CCR1 = REGISTERS[SERVO1_HIGH] << 8 | REGISTERS[SERVO1_LOW];
  TIM2->CCR2 = REGISTERS[SERVO2_HIGH] << 8 | REGISTERS[SERVO2_LOW];
  TIM3->CCR1 = REGISTERS[SERVO3_HIGH] << 8 | REGISTERS[SERVO3_LOW];
}

void WritePins() {
  HAL_GPIO_WritePin(TOGGLE1_GPIO_Port, TOGGLE1_Pin, REGISTERS[TOGGLE] & 0b00000001);
  HAL_GPIO_WritePin(TOGGLE2_GPIO_Port, TOGGLE2_Pin, REGISTERS[TOGGLE] & 0b00000010);
  HAL_GPIO_WritePin(TOGGLE3_GPIO_Port, TOGGLE3_Pin, REGISTERS[TOGGLE] & 0b00000100);
  HAL_GPIO_WritePin(TOGGLE4_GPIO_Port, TOGGLE4_Pin, REGISTERS[TOGGLE] & 0b00001000);
  HAL_GPIO_WritePin(TOGGLE5_GPIO_Port, TOGGLE5_Pin, REGISTERS[TOGGLE] & 0b00010000);
  HAL_GPIO_WritePin(TOGGLE6_GPIO_Port, TOGGLE6_Pin, REGISTERS[TOGGLE] & 0b00100000);
  HAL_GPIO_WritePin(TOGGLE7_GPIO_Port, TOGGLE7_Pin, REGISTERS[TOGGLE] & 0b01000000);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  WriteServos();

  HAL_I2C_EnableListen_IT(&hi2c1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	// Read PG - probably a better way to do this
	HAL_Delay(10);

	REGISTERS[REGPG] = (HAL_GPIO_ReadPin(REGPG1_GPIO_Port, REGPG1_Pin)) |
		               (HAL_GPIO_ReadPin(REGPG2_GPIO_Port, REGPG2_Pin) << 1) |
                       (HAL_GPIO_ReadPin(REGPG3_GPIO_Port, REGPG3_Pin) << 2) |
                       (HAL_GPIO_ReadPin(REGPG4_GPIO_Port, REGPG4_Pin) << 3) |
                       (HAL_GPIO_ReadPin(REGPG5_GPIO_Port, REGPG5_Pin) << 4) |
                       (HAL_GPIO_ReadPin(REGPG6_GPIO_Port, REGPG6_Pin) << 5) |
                       (HAL_GPIO_ReadPin(REGPG7_GPIO_Port, REGPG7_Pin) << 6);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 66;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 32-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 32-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TOGGLE5_Pin|TOGGLE6_Pin|TOGGLE2_Pin|TOGGLE1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TOGGLE7_Pin|TOGGLE4_Pin|TOGGLE3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : TOGGLE5_Pin TOGGLE6_Pin TOGGLE2_Pin TOGGLE1_Pin */
  GPIO_InitStruct.Pin = TOGGLE5_Pin|TOGGLE6_Pin|TOGGLE2_Pin|TOGGLE1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : REGPG5_Pin REGPG6_Pin REGPG1_Pin */
  GPIO_InitStruct.Pin = REGPG5_Pin|REGPG6_Pin|REGPG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TOGGLE7_Pin TOGGLE4_Pin TOGGLE3_Pin */
  GPIO_InitStruct.Pin = TOGGLE7_Pin|TOGGLE4_Pin|TOGGLE3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : REGPG7_Pin REGPG4_Pin REGPG3_Pin REGPG2_Pin */
  GPIO_InitStruct.Pin = REGPG7_Pin|REGPG4_Pin|REGPG3_Pin|REGPG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
