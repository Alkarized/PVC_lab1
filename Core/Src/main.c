/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t RED = GPIO_PIN_15;
uint16_t YELLOW = GPIO_PIN_14;
uint16_t GREEN = GPIO_PIN_13;
#define COMMON_DELAY 50

void wait(uint32_t duration)
{
	uint32_t startTime = HAL_GetTick();
	while((HAL_GetTick() - startTime) < duration){}
}

void turn_all_off()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}


void turn_light_on(uint16_t light)
{
	turn_all_off();
	HAL_GPIO_WritePin(GPIOD, light, GPIO_PIN_SET);
}

void wait_and_change(uint32_t *duration, uint32_t *redDuration, uint8_t *flag){
	uint32_t startTime = 0;
	startTime = HAL_GetTick();
	while((HAL_GetTick() - startTime) < *duration)
	{
		GPIO_PinState measure_1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
		wait(COMMON_DELAY);
		GPIO_PinState measure_2 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);

		if((!measure_1 && !measure_2) && *flag == 0) {
			*redDuration = *redDuration / 4;
			*flag = 1;
		}
	}
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
  /* USER CODE BEGIN 2 */

  //uint32_t startTime = 0;
  uint32_t duration = 1000;
  uint32_t blink_duration = 500;
  uint32_t duration_red = 4 * duration;
  uint32_t duration_yellow = 1000;
  uint8_t button_flag = 0;
  uint8_t blink_count = 3;

  turn_all_off();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  turn_light_on(RED);

	  wait_and_change(&duration_red, &duration_red, &button_flag);

//	  startTime = HAL_GetTick();
//	  while((HAL_GetTick() - startTime) < duration_red)
//	  {
//		  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0 && button_flag == 0) {
//			  duration_red = duration_red / 4;
//			  button_flag = 1;
//		  }
//	  }

	  //Red of, Green on
	  turn_light_on(GREEN);

	  wait(duration);

	  duration_red = 4 * duration;
	  button_flag = 0;

	  for(uint32_t i = 0; i < blink_count; i++){
		turn_all_off();

		wait_and_change(&blink_duration, &duration_red, &button_flag);


		/*startTime = HAL_GetTick();
		while((HAL_GetTick() - startTime) < blink_duration)
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0 && button_flag == 0) {
				duration_red = duration_red / 4;
				button_flag = 1;
			}
		}*/

		turn_light_on(GREEN);
		wait_and_change(&blink_duration, &duration_red, &button_flag);
		/*startTime = HAL_GetTick();
		while((HAL_GetTick() - startTime) < blink_duration)
		{
		  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0 && button_flag == 0) {
			  duration_red = duration_red / 4;
			  button_flag = 1;
		  }
		}*/
	  }

	  turn_light_on(YELLOW);

	  wait_and_change(&duration_yellow, &duration_red, &button_flag);

	  /*startTime = HAL_GetTick();
	  while((HAL_GetTick() - startTime) < duration_yellow)
	  {
		  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0 && button_flag == 0) {
			  duration_red = duration_red / 4;
			  button_flag = 1;
		  }
	  }*/

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
