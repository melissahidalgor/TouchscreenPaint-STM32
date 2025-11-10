/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
#include "macros_utiles.h"
#include "I2C_LCDT.h"
#include "STMEP811.h"
#include "lcd_driver.h"
#include "stdio.h"
#include "keyboard.h"

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
/* Definitions for paintTask */
osThreadId_t paintTaskHandle;
const osThreadAttr_t paintTask_attributes = {
  .name = "paintTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for selectColorTask */
osThreadId_t selectColorTaskHandle;
const osThreadAttr_t selectColorTask_attributes = {
  .name = "selectColorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for dummyTask */
osThreadId_t dummyTaskHandle;
const osThreadAttr_t dummyTask_attributes = {
  .name = "dummyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for colorPaintMutex */
osMutexId_t colorPaintMutexHandle;
const osMutexAttr_t colorPaintMutex_attributes = {
  .name = "colorPaintMutex"
};
/* USER CODE BEGIN PV */
uint16_t lineColor = 0xF81B;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartPaintTask(void *argument);
void StartSelectColorTask(void *argument);
void StartDummyTask(void *argument);

/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */

  // ----- LCD Configurations ----- //
  	I2C_init();
  	STMPE811_Touch_Enable();
  	LCD_InitGPIO();
  	SPI_Config();
  	LCD_InitSerialInterface();

  // ----- LCD Interface Init ----- //
  	LCD_WriteChar('X', 0xFFFF, 0x0, 185, 0);
  	LCD_WriteChar('Y', 0xFFFF, 0x0, 185, 15);
  	LCD_writeString("310027", 10, 0);
  	LCD_FillRect(75, 0, 90, 15, lineColor);


  	// ----- Keyboard Configurations ----- //
  	keyboard_config();

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of colorPaintMutex */
  colorPaintMutexHandle = osMutexNew(&colorPaintMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of paintTask */
  paintTaskHandle = osThreadNew(StartPaintTask, NULL, &paintTask_attributes);

  /* creation of selectColorTask */
  selectColorTaskHandle = osThreadNew(StartSelectColorTask, NULL, &selectColorTask_attributes);

  /* creation of dummyTask */
  dummyTaskHandle = osThreadNew(StartDummyTask, NULL, &dummyTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartPaintTask */
/**
  * @brief  Function implementing the paintTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartPaintTask */
void StartPaintTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	uint16_t TouchX,TouchY;
	char lcd_data[7];

  /* Infinite loop */
	for(;;)
	{
		if(isTouched() == touched)
		{
			if(osMutexAcquire(colorPaintMutexHandle, osWaitForever)== osOK)
			{
				getTouchValue(&TouchX,&TouchY);

				sprintf(lcd_data,"X %d ",TouchX);
				LCD_writeString(lcd_data, 185, 0);

				sprintf(lcd_data,"Y %d ",TouchY);
				LCD_writeString(lcd_data, 185, 15);

				//LCD_DrawPixel(TouchX, TouchY, lineColor);
				LCD_FillRect(TouchX-1, TouchY-1, TouchX, TouchY, lineColor);

				osMutexRelease(colorPaintMutexHandle);
				osDelay(5);
			}
		}
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSelectColorTask */
/**
* @brief Function implementing the selectColorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSelectColorTask */
void StartSelectColorTask(void *argument)
{
  /* USER CODE BEGIN StartSelectColorTask */
	uint8_t positionRGB = 0, R, G, B;
	uint8_t colorRGB[6];
	char chr;
  /* Infinite loop */
  for(;;)
  {
    if(keypad_tecla() == 'C')
	{
    	if(osMutexAcquire(colorPaintMutexHandle, osWaitForever)== osOK)
    	{
			LCD_writeString("      ", 10, 0);
			LCD_FillRect(75, 0, 90, 15, 0xFFFF);
			while(positionRGB < 6)
			{
				chr = keypad_tecla();
				osDelay(200);			// Debouncing delay
				if(chr >='0' && chr <= '9')
				{
					LCD_WriteChar(chr, 0xFFFF, 0x0, positionRGB*10+10, 0);

					colorRGB[positionRGB] = chr - 48;
					positionRGB ++;
				}
			}
			R = colorRGB[0]*10 + colorRGB[1];
			G = colorRGB[2]*10 + colorRGB[3];
			B = colorRGB[4]*10 + colorRGB[5];
			lineColor = (R << 11) | (G << 5) | B;
			positionRGB = 0;
			LCD_FillRect(75, 0, 90, 15, lineColor);

			osMutexRelease(colorPaintMutexHandle);
			osDelay(100);
    	} // if mutex
	} // if 'C'
  }// for
  /* USER CODE END StartSelectColorTask */
}

/* USER CODE BEGIN Header_StartDummyTask */
/**
* @brief Function implementing the dummyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDummyTask */
void StartDummyTask(void *argument)
{
  /* USER CODE BEGIN StartDummyTask */
  /* Infinite loop */
  for(;;)
  {
  }
  /* USER CODE END StartDummyTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
