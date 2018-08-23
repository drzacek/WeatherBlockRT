
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "myfuncs.h"
#include "usbcommands.h"
#include "ili9341_LCD.h"
#include "printtext.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile short microsecondCounter;
extern DATETIME_STRUCT dateTime;
unsigned char USBtext[256] = "";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

// Micro second waitd
void Delay_us(short us);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

// Fonts
// 8x8 Font
//extern unsigned char bgi8x8font[8*256];

int temp;
int humi;

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	microsecondCounter++;
//}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	microsecondCounter = 0;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */


  // Timer and Timer Interrupt init

  //HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
  //HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  //HAL_TIM_Base_Start_IT(&htim1);


  // Define variables




   ILI9341_Init();

   HAL_Delay(500);

   ILI9341_Fill_Screen(0x1863);
   setCurrentBGColor(0x1863);
   setCurrentColor(GREEN);

   ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


   //setCurrentDate(3, 15, 8, 18);
   //setCurrentTime(21,52,20);

   unsigned char tempstring[256];
   char LED = 1;

   HAL_Delay(150);


   char taskCounter = 0;


	sprintf(tempstring, "!!!  loriland.ml !!!");
	outtextxy32x(0, 0, tempstring, 0);

	sprintf(tempstring, "Temp:");
	outtextxy32x(2, 3, tempstring, 1);

	sprintf(tempstring, "Humidity:");
	outtextxy32x(2, 4, tempstring, 1);

	short frameColor = BLUE;

	//ILI9341_Draw_Rectangle(5, 50, 315, 235, WHITE);
	ILI9341_Draw_Horizontal_Line(20, 31, 280, frameColor);
	ILI9341_Draw_Horizontal_Line(20, 96, 280, frameColor);
	ILI9341_Draw_Vertical_Line(300, 31, 64, frameColor);
	ILI9341_Draw_Vertical_Line(20, 31, 64, frameColor);

//	ILI9341_Draw_Horizontal_Line(20, 235, 280, frameColor);
//	ILI9341_Draw_Horizontal_Line(20, 170, 280, frameColor);
//	ILI9341_Draw_Vertical_Line(300, 170, 64, frameColor);
//	ILI9341_Draw_Vertical_Line(20, 170, 64, frameColor);

	ILI9341_Draw_Rectangle(20, 170, 280, 64, BLUE);
	ILI9341_Draw_Rectangle(21, 171, 278, 62, 0x1863);

	unsigned char tempHistory[220];

	memset(&tempHistory, 0, 220);

   while (1)
   {


	   switch(taskCounter)
	   {
		   case 0:
			   // TOGGLE LED
			   if(LED == 1)
			   {
				   LED = 0;
				   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
				   //ILI9341_Fill_Screen(CYAN);
			   }
			   else
			   {
				   LED = 1;
				   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
				   //ILI9341_Fill_Screen(NAVY);
			   }
			   break;

		   case 1:
			   // DHT22
		   	   {
		   		   static int delayCounter = 8;

		   		   if(delayCounter == 10)
				   {
		   			   delayCounter = 0;

		   			   readDHT22Data();
				   	   DHT22_DecodeReadings();
				   }

		   		   delayCounter++;
		   	   }

			   humi = DHT22_GetHumidity();
			   temp = DHT22_GetTemperature();


			   if( (temp - 220) < 0)
				   tempHistory[0] = 0;
			   else
				   tempHistory[0] = (temp - 220)%60; 	// 0 at 22.0 C, max at 28.3C
			   break;

		   case 2:
			   // RealTime Clock

			   getCurrentTime();

			   break;

		   case 3:
			   getCurrentDate();
			   break;

		   case 4:
			   // Display





			   // Time
			   sprintf(tempstring, "%02d:%02d:%02d", dateTime.dt_hours, dateTime.dt_minutes, dateTime.dt_seconds);
			   outtextxy32x(6, 1, tempstring, 0);

			   // Date
			   sprintf(tempstring, "%02d.%02d.%04d", dateTime.dt_day, dateTime.dt_month, dateTime.dt_year);
			   outtextxy32x(5, 2, tempstring, 0);

			   sprintf(tempstring, "%02d.%dC", temp/10, temp%10);
			   outtextxy32x(12, 3, tempstring, 1);

			   sprintf(tempstring, "%02d.%d%%", humi/10, humi%10);
			   outtextxy32x(12, 4, tempstring, 1);

			   //outtextxy32x(5, 5, "NEW XBM STRING");
			   //outtextxy32x(5, 6, "does it look better?");
			   /*
			   sprintf(tempstring, "Temperature:");
			   outtextxy(3, 18, tempstring);

			   sprintf(tempstring, "Humidity:");
			   outtextxy(3, 20, tempstring);

			   sprintf(tempstring, "\xb0 C");
			   outtextxy(23, 18, tempstring);

			   sprintf(tempstring, "%%");
			   outtextxy(23, 20, tempstring);

			   sprintf(tempstring, "%02d.%d", temp/10, temp%10);
			   outtextxy16(19, 9, tempstring);

			   sprintf(tempstring, "%02d.%d", humi/10, humi%10);
			   outtextxy16(19, 10, tempstring);
			   */

			   break;

		   case 5:
			   // MQTT

			   // Temperature Graph

			   // Clean graph
				ILI9341_Draw_Rectangle(21, 171, 278, 62, 0x1863);

				// Print temperature
				for(int ti = 0; ti < 220; ti++)
				{

					ILI9341_Draw_Pixel(277 - ti, 232 - tempHistory[ti], WHITE);
				}


				// Shift array
				for(int thi = 219; thi > 0; thi--)
				{
					tempHistory[thi] = tempHistory[thi-1];
				}

			   break;
	   }

	   // Serial Commands (from PC via Virtual Com Port)
	   if(!readUSBSerial(USBtext))
	   {
		   // Handle the commands
		   int command;
		   command = DecodeCommand(USBtext);

		   DoCommand(command);
	   }

	   taskCounter++;

	   if(taskCounter > 5)
	   {
		   taskCounter = 0;
	   }

	   HAL_Delay(20);







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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 36;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED13_GPIO_Port, LED13_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RESET_Pin|LCD_CS_Pin|LCD_DCRS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED13_Pin */
  GPIO_InitStruct.Pin = LED13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DHT22_IN_Pin */
  GPIO_InitStruct.Pin = DHT22_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT22_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RESET_Pin LCD_CS_Pin LCD_DCRS_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin|LCD_CS_Pin|LCD_DCRS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
