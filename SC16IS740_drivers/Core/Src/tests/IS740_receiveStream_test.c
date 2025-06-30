/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SC16IS740.h"
#include <stdint.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
IS740error_t SendToBridge(uint8_t addr, uint8_t *buffer, uint8_t size);
IS740error_t ReadFromBridge(uint8_t addr, uint8_t *buffer, uint8_t size);
void delayMs(uint32_t t);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
	IS740handle_t bridge;
	bridge.readFunc= ReadFromBridge;
	bridge.writeFunc = SendToBridge;
	bridge.config.baudRate = 9600;
	bridge.config.parity = IS740_PARITY_NONE;
	bridge.config.stopBits = IS740_STOPLEN_1;
	bridge.config.wordLen = IS740_WORDLEN_8;

	uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
	IS740_setBaudRate(&bridge, pclk1);
	IS740_LoopbackControl(&bridge, ENABLE);
	IS740_init(&bridge);

	uint8_t string[] = "Hello World!\n";
	uint8_t rxbuf[64];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
	  delayMs(150);

	  IS740_transmitStream(&bridge, string, sizeof(string));

	  while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
	  delayMs(150);
	  IS740_receiveStream(&bridge, rxbuf, sizeof(string));
	  printf("DATA: %s", rxbuf);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */
IS740error_t SendToBridge(uint8_t addr, uint8_t *buffer, uint8_t size){
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write(&hi2c1, 0x90, addr, 1, buffer, size, 1000);
	if(status==HAL_TIMEOUT)
		return IS740_ERROR_TIMEOUT;
	else if(status == HAL_ERROR)
		return IS740_ERROR_UNKNOWN;

	return IS740_ERROR_NONE;
}
IS740error_t ReadFromBridge(uint8_t addr, uint8_t *buffer, uint8_t size){
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(&hi2c1, 0x90, addr, 1, buffer, size, 1000);
	if(status==HAL_TIMEOUT)
		return IS740_ERROR_TIMEOUT;
	else if(status == HAL_ERROR)
		return IS740_ERROR_UNKNOWN;

	return IS740_ERROR_NONE;
}

void delayMs(uint32_t t){
	uint32_t temp = HAL_RCC_GetSysClockFreq();
	// 13.2 clocks/loop * s/clock = s/loop
	// loops = s/(13.2*sysclk) = ms/(13200*sysclk)
	temp = t*temp/13200;
	for(int i=0;i<temp;i++);
}
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
