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

#include <stdio.h>
#include <string.h>

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

ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

// 4096 = 2^12 = 0-4095
uint32_t adc_data = 0; //0-4095
uint32_t pwm_value = 0; // 0-4095

float duty_cycle = 0.0f;

char uart_buffer[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

	/* USER CODE END 2 */

	/* Initialize leds */
	BSP_LED_Init(LED2);

	/* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (HAL_ADC_Start(&hadc1) == HAL_OK) {
			if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
				adc_data = HAL_ADC_GetValue(&hadc1);

				// Convert ADC value to PWM value
				pwm_value = (adc_data * 999U) / 4095U;

				// Update PWM duty cycle
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_value);

				// Calculate duty cycle percentage
				duty_cycle = (float) pwm_value * 100.0f / 1000.0f;

				// Format UART message
				snprintf(uart_buffer, sizeof(uart_buffer),
						"ADC: %lu | PWM: %lu | Duty: %.1f%%\r\n", adc_data,
						pwm_value, duty_cycle);

				// Send UART message
				HAL_UART_Transmit(&huart2, (uint8_t*) uart_buffer,
						strlen(uart_buffer),
						HAL_MAX_DELAY);
			}

			HAL_ADC_Stop(&hadc1);
		}

		HAL_Delay(100);
	}

//	ADC: 1892 | PWM: 461 | Duty: 46.1%
//	ADC: 1737 | PWM: 423 | Duty: 42.3%
//	ADC: 1596 | PWM: 389 | Duty: 38.9%
//	ADC: 1446 | PWM: 352 | Duty: 35.2%
//	ADC: 1288 | PWM: 314 | Duty: 31.4%
//	ADC: 1213 | PWM: 295 | Duty: 29.5%
//	ADC: 1202 | PWM: 293 | Duty: 29.3%
//	ADC: 1226 | PWM: 299 | Duty: 29.9%
//	ADC: 1209 | PWM: 294 | Duty: 29.4%
//	ADC: 1201 | PWM: 292 | Duty: 29.2%
//	ADC: 1208 | PWM: 294 | Duty: 29.4%
//	ADC: 1196 | PWM: 291 | Duty: 29.1%
//	ADC: 1166 | PWM: 284 | Duty: 28.4%
//	ADC: 1082 | PWM: 263 | Duty: 26.3%
//	ADC: 967 | PWM: 235 | Duty: 23.5%
//	ADC: 781 | PWM: 190 | Duty: 19.0%
//	ADC: 593 | PWM: 144 | Duty: 14.4%
//	ADC: 374 | PWM: 91 | Duty: 9.1%
//	ADC: 123 | PWM: 30 | Duty: 3.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 3 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 5 | PWM: 1 | Duty: 0.1%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 3 | PWM: 0 | Duty: 0.0%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 4 | PWM: 0 | Duty: 0.0%
//	ADC: 6 | PWM: 1 | Duty: 0.1%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 0 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 1 | PWM: 0 | Duty: 0.0%
//	ADC: 2 | PWM: 0 | Duty: 0.0%
//	ADC: 38 | PWM: 9 | Duty: 0.9%
//	ADC: 124 | PWM: 30 | Duty: 3.0%
//	ADC: 213 | PWM: 51 | Duty: 5.1%
//	ADC: 304 | PWM: 74 | Duty: 7.4%
//	ADC: 380 | PWM: 92 | Duty: 9.2%
//	ADC: 463 | PWM: 112 | Duty: 11.2%
//	ADC: 547 | PWM: 133 | Duty: 13.3%
//	ADC: 640 | PWM: 156 | Duty: 15.6%
//	ADC: 733 | PWM: 178 | Duty: 17.8%
//	ADC: 816 | PWM: 199 | Duty: 19.9%
//	ADC: 930 | PWM: 226 | Duty: 22.6%
//	ADC: 1036 | PWM: 252 | Duty: 25.2%
//	ADC: 1090 | PWM: 265 | Duty: 26.5%
//	ADC: 1078 | PWM: 262 | Duty: 26.2%
//	ADC: 1094 | PWM: 266 | Duty: 26.6%
//	ADC: 1082 | PWM: 263 | Duty: 26.3%
//	ADC: 1088 | PWM: 265 | Duty: 26.5%
//	ADC: 1112 | PWM: 271 | Duty: 27.1%
//	ADC: 1187 | PWM: 289 | Duty: 28.9%
//	ADC: 1312 | PWM: 320 | Duty: 32.0%
//	ADC: 1460 | PWM: 356 | Duty: 35.6%
//	ADC: 1627 | PWM: 396 | Duty: 39.6%
//	ADC: 1811 | PWM: 441 | Duty: 44.1%
//	ADC: 1993 | PWM: 486 | Duty: 48.6%
//	ADC: 2130 | PWM: 519 | Duty: 51.9%
//	ADC: 2228 | PWM: 543 | Duty: 54.3%
//	ADC: 2311 | PWM: 563 | Duty: 56.3%
//	ADC: 2319 | PWM: 565 | Duty: 56.5%
//	ADC: 2328 | PWM: 567 | Duty: 56.7%
//	ADC: 2334 | PWM: 569 | Duty: 56.9%
//	ADC: 2326 | PWM: 567 | Duty: 56.7%
//	ADC: 2389 | PWM: 582 | Duty: 58.2%
//	ADC: 2475 | PWM: 603 | Duty: 60.3%
//	ADC: 2585 | PWM: 630 | Duty: 63.0%
//	ADC: 2732 | PWM: 666 | Duty: 66.6%
//	ADC: 2934 | PWM: 715 | Duty: 71.5%
//	ADC: 3145 | PWM: 767 | Duty: 76.7%
//	ADC: 3361 | PWM: 819 | Duty: 81.9%
//	ADC: 3532 | PWM: 861 | Duty: 86.1%
//	ADC: 3618 | PWM: 882 | Duty: 88.2%
//	ADC: 3623 | PWM: 883 | Duty: 88.3%
//	ADC: 3630 | PWM: 885 | Duty: 88.5%
//	ADC: 3600 | PWM: 878 | Duty: 87.8%
//	ADC: 3637 | PWM: 887 | Duty: 88.7%
//	ADC: 3686 | PWM: 899 | Duty: 89.9%
//	ADC: 3800 | PWM: 927 | Duty: 92.7%
//	ADC: 3962 | PWM: 966 | Duty: 96.6%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4094 | PWM: 998 | Duty: 99.8%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4090 | PWM: 997 | Duty: 99.7%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4094 | PWM: 998 | Duty: 99.8%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4064 | PWM: 991 | Duty: 99.1%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4094 | PWM: 998 | Duty: 99.8%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4094 | PWM: 998 | Duty: 99.8%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%
//	ADC: 4095 | PWM: 999 | Duty: 99.9%




	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 83;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 999;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
