/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define MAX_LED 4
#define MAX_LED_MATRIX 8
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
int index_led = 0;
int led_buffer[4] = {1, 5, 0, 8};

int hour = 15, minute = 8, second = 50;

/* USER CODE BEGIN PV */
int index_led_matrix = 0;
uint8_t matrix_buffer[8] = {
    0x18,  // 00011000
    0x3C,  // 00111100
    0x66,  // 01100110
    0x66,  // 01100110
    0x7E,  // 01111110
    0x66,  // 01100110
    0x66,  // 01100110
    0x00   // 00000000
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void updateClockBuffer(void);
void updateLEDMatrix(int index);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG(int num){
	// reset hết v�? off (tất cả các chân segment = SET cho common anode)
	HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, e_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, SET);
	HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, SET);

	switch (num){
	case 0:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, e_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, e_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, e_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, e_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOB, a_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, b_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, c_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, d_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, f_7seg_Pin, RESET);
		HAL_GPIO_WritePin(GPIOB, g_7seg_Pin, RESET);
		break;
	}
}

void update7SEG(int index){
	HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);

	switch(index){
	case 0: HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
	        display7SEG(led_buffer[0]); break;
	case 1: HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
	        display7SEG(led_buffer[1]); break;
	case 2: HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
	        display7SEG(led_buffer[2]); break;
	case 3: HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
	        display7SEG(led_buffer[3]); break;
	}
}

void updateClockBuffer(void){
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
}

void updateLEDMatrix(int col){
    HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin|
                             ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & (1 << col)) ? GPIO_PIN_RESET : GPIO_PIN_SET);

    switch(col){
    case 0: HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_RESET); break;
    case 1: HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_RESET); break;
    case 2: HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_RESET); break;
    case 3: HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_RESET); break;
    case 4: HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_RESET); break;
    case 5: HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_RESET); break;
    case 6: HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_RESET); break;
    case 7: HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_RESET); break;
    }
}


//void shiftLeft(){
//    for(int i = 0; i < MAX_LED_MATRIX - 1; i++){
//        matrix_buffer[i] = matrix_buffer[i+1];
//    }
//    matrix_buffer[MAX_LED_MATRIX-1] = 0x00;
//}

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  setTimer1(25);   // 250ms
  setTimer2(100);
  setTimer3(2);
  //setTimer4(20);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  // update 7seg
	    if(timer1_flag == 1){
	      timer1_flag = 0;
	      setTimer1(25);
	      update7SEG(index_led);
	      index_led = (index_led + 1) % MAX_LED;
	    }

	    // Update clock, dot
	    if(timer2_flag == 1){
	      timer2_flag = 0;
	      setTimer2(100);

	      second++;
	      if(second >= 60){
	        second = 0;
	        minute++;
	      }
	      if(minute >= 60){
	        minute = 0;
	        hour++;
	      }
	      if(hour >= 24){
	        hour = 0;
	      }

	      updateClockBuffer();
	      HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
	    }

	    // update led matrix
	    if(timer3_flag == 1){
	        timer3_flag = 0;
	        setTimer3(2);
	        updateLEDMatrix(index_led_matrix);
	        index_led_matrix = (index_led_matrix + 1) % MAX_LED_MATRIX;
	    }

//	    // animation
//	    if(timer4_flag == 1){
//	        timer4_flag = 0;
//	        setTimer4(200);
//	        shiftLeft();
//	    }
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

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a_7seg_Pin|b_7seg_Pin|c_7seg_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|d_7seg_Pin|e_7seg_Pin|f_7seg_Pin
                          |g_7seg_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a_7seg_Pin b_7seg_Pin c_7seg_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin d_7seg_Pin e_7seg_Pin f_7seg_Pin
                           g_7seg_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = a_7seg_Pin|b_7seg_Pin|c_7seg_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|d_7seg_Pin|e_7seg_Pin|f_7seg_Pin
                          |g_7seg_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM2){
    timerRun();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
