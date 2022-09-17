/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "u8g2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Attention: Don't config LCD pins in cubeMX. Do the configuration as follows :
#define LED1_PIN                         GPIO_PIN_6
#define LED2_PIN                         GPIO_PIN_7
#define LED_GPIO_PORT                    GPIOB
#define LED_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LCD_RST_1       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  GPIO_PIN_SET)
#define LCD_RST_0       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  GPIO_PIN_RESET)
#define LCD_RS_1	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define LCD_RS_0	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define LCD_SCLK_1      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define LCD_SCLK_0      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
#define LCD_SID_1       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define LCD_SID_0       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
#define CONTROL_GPIO_CLK_ENABLE() 		__HAL_RCC_GPIOB_CLK_ENABLE()
#define CONTROL_GPIO_CLK_DISABLE() 		__HAL_RCC_GPIOB_CLK_DISABLE()
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static u8g2_t u8g2;

void delay_us(uint32_t time) {
	uint32_t i = 0;

	while (time--) {
		i = 10;
		while (i--)
			;
	}
}

void delay_ms(uint32_t time) {
	uint32_t i = 0;

	while (time--) {
		i = 12000;
		while (i--)
			;
	}
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8,
U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
U8X8_UNUSED void *arg_ptr) {
	GPIO_InitTypeDef gpioinitstruct;

	switch (msg) {

	//Function which implements a delay, arg_int contains the amount of ms
	case U8X8_MSG_GPIO_AND_DELAY_INIT:

		//__HAL_RCC_GPIOB_CLK_ENABLE();
		CONTROL_GPIO_CLK_ENABLE();
		/* Configure the GPIO_LED pin */
		gpioinitstruct.Pin = GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10
				| GPIO_PIN_9;
		gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull = GPIO_NOPULL;
		gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &gpioinitstruct);

		HAL_GPIO_WritePin(LED_GPIO_PORT,
		GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9, GPIO_PIN_SET);

		break;
		//Function which implements a delay, arg_int contains the amount of ms
	case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
		break;
		//Function which delays 10us
	case U8X8_MSG_DELAY_10MICRO:
		delay_us(10);
		break;
		//Function which delays 100ns
	case U8X8_MSG_DELAY_100NANO:
		__NOP();

		break;
		//Function to define the logic level of the clockline
	case U8X8_MSG_GPIO_SPI_CLOCK:
		if (arg_int)
			LCD_SCLK_1;
		else
			LCD_SCLK_0;

		break;
		//Function to define the logic level of the data line to the display
	case U8X8_MSG_GPIO_SPI_DATA:
		if (arg_int)
			LCD_SID_1;
		else
			LCD_SID_0;

		break;

		// Function to define the logic level of the CS line
	case U8X8_MSG_GPIO_CS1:
		if (arg_int)
			LCD_RS_1;
		else
			LCD_RS_0;

		break;
		//Function to define the logic level of the Data/ Command line
	case U8X8_MSG_GPIO_DC:

		break;
		//Function to define the logic level of the RESET line
	case U8X8_MSG_GPIO_RESET:
		if (arg_int)
			LCD_RST_1;
		else
			LCD_RST_0;

		break;

	default:
		return 0; //A message was received which is not implemented, return 0 to indicate an error
	}

	return 1; // command processed successfully.
}

void BSP_LED_Init(void) {
	GPIO_InitTypeDef gpioinitstruct;

	/* Enable the GPIO_LED Clock */
	LED_GPIO_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	gpioinitstruct.Pin = LED1_PIN | LED2_PIN;
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull = GPIO_NOPULL;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(LED_GPIO_PORT, &gpioinitstruct);

	/* Reset PIN to switch off the LED */
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
}

void BSP_LED_DeInit(void) {
	GPIO_InitTypeDef gpio_init_structure;

	/* Turn off LED */
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
	/* DeInit the GPIO_LED pin */
	gpio_init_structure.Pin = LED1_PIN | LED2_PIN;
	HAL_GPIO_DeInit(LED_GPIO_PORT, gpio_init_structure.Pin);
}

void BSP_LED1_On(void) {
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

void BSP_LED1_Off(void) {
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void BSP_LED1_Toggle(void) {
	HAL_GPIO_TogglePin(LED_GPIO_PORT, LED1_PIN);
}

void BSP_LED2_On(void) {
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
}

void BSP_LED2_Off(void) {
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
}

void BSP_LED2_Toggle(void) {
	HAL_GPIO_TogglePin(LED_GPIO_PORT, LED2_PIN);
}

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
	/* USER CODE BEGIN 2 */
	/* Configure LED */
	BSP_LED_Init();
	u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi,
			u8g2_gpio_and_delay_stm32); // init u8g2 structure
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	HAL_Delay(1000);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_Delay(1000);
		BSP_LED2_Toggle();
		BSP_LED1_Toggle();

		// Hello World
		u8g2_ClearBuffer(&u8g2);
		u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
		u8g2_DrawStr(&u8g2, 0, 15, "Hello World!");
		u8g2_SendBuffer(&u8g2);
		HAL_Delay(3000);

		// Draw Box
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawBox(&u8g2, 10, 10, 108, 44);
		u8g2_SendBuffer(&u8g2);
		HAL_Delay(3000);

		// Draw Circle
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawCircle(&u8g2, 64, 32, 5, U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2, 64, 32, 10, U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2, 64, 32, 15, U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2, 64, 32, 20, U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2, 64, 32, 25, U8G2_DRAW_ALL);
		u8g2_SendBuffer(&u8g2);
		HAL_Delay(3000);

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
