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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
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

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
// GARDE UNIQUEMENT CE BLOC UNE SEULE FOIS
uint8_t __CIPSEND[20]  =  "";
uint8_t RxDataBuffer[40];
uint8_t Exti0=0;
uint8_t UART_DMA_DataIsReady = 0;
char DMA_Buffer[256];
char CPU_Buffer[256];
char response[50];
char result[50];
char __OK[] = "OK";
char BUFFER_TO_SEND[100];
uint16_t ADCVal = 0;

DMA_HandleTypeDef hdma_adc1;
/* USER CODE END PV */










/* USER CODE BEGIN PV */
uint8_t __CIPSEND[20]  =  "";       // Buffer for sending data, for an AT command
uint8_t RxDataBuffer[40];           // Buffer for receiving data
uint8_t Exti0=0;
uint8_t UART_DMA_DataIsReady = 0;   // Flag to indicate if UART DMA data is ready
char DMA_Buffer[256];               // Buffer for DMA data
char CPU_Buffer[256];               // Buffer for CPU data processing
char response[50];                  // Buffer for storing response data
char result[50];                    // Buffer for storing result data
char __OK[] = "OK";                 // String constant for "OK" response
char BUFFER_TO_SEND[100];           // Buffer for data to be sent
uint16_t ADCVal = 0;                 // Variable to store ADC value

/* ========== AJOUT DES DECLARATIONS MANQUANTES ========== */
DMA_HandleTypeDef hdma_adc1;        // Déclaration du handle DMA pour ADC1
/* ========== FIN AJOUT ========== */
/* USER CODE END PV */

/* USER CODE BEGIN PV */
uint8_t __CIPSEND[20]  =  "";       // Buffer for sending data, for an AT command
uint8_t RxDataBuffer[40];           // Buffer for receiving data
uint8_t Exti0=0;
uint8_t UART_DMA_DataIsReady = 0;   // Flag to indicate if UART DMA data is ready
char DMA_Buffer[256];               // Buffer for DMA data
char CPU_Buffer[256];               // Buffer for CPU data processing
char response[50];                  // Buffer for storing response data
char result[50];                    // Buffer for storing result data
char __OK[] = "OK";                 // String constant for "OK" response
char BUFFER_TO_SEND[100];           // Buffer for data to be sent
uint16_t ADCVal = 0;                 // Variable to store ADC value
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void ESP8266_Init (void);
void ESP8266_SendServer (void);
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  ESP8266_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	    while (Exti0 == 0){}
	    ESP8266_SendServer();
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void ESP8266_Init (void)
{
  // Send reset command to ESP8266 and store the result
  ESP8266_SendCommand (__RST);
  // Wait for 2 seconds to allow the module to reset
  HAL_Delay(2000);
  // Send AT command to check if the module is responding
  ESP8266_SendCommand (__AT);
  // Set the WiFi mode (e.g., station mode)
  ESP8266_SendCommand (__CWMODE);
  // Connect to the specified WiFi network using provided credentials
  ESP8266_Connect_To_WiFi(WIFI_NAME, WIFI_PASSWORD);
  // Wait for 4 seconds to ensure the connection is established
  HAL_Delay(4000);
}


// Function to send data to the server
void ESP8266_SendServer (void)
{
  // Start ADC conversion
  HAL_ADC_Start(&hadc1);
  // Poll for ADC conversion completion with a timeout of 10 units
  HAL_ADC_PollForConversion(&hadc1, 10);
  // Get the ADC value
  ADCVal = HAL_ADC_GetValue(&hadc1);
  // Stop ADC conversion
  HAL_ADC_Stop(&hadc1);

  // Start a TCP connection
  ESP8266_SendCommand (__CIPSTART);

  // Clear the buffer to send
  memset (BUFFER_TO_SEND, '\0', sizeof(BUFFER_TO_SEND));
  // Format the data to send, including the server IP and ADC value
  sprintf(BUFFER_TO_SEND, "%s=%d\r\n", __SERVER_IP_WRITE, ADCVal);
  // Format the CIPSEND command with the length of the data to send
  sprintf((char*)__CIPSEND, "AT+CIPSEND=%d\r\n", strlen(BUFFER_TO_SEND));
  // Send the CIPSEND command to the ESP8266
  ESP8266_SendCommand (__CIPSEND);
  // Send the actual data to the ESP8266
  ESP8266_SendCommand((uint8_t*)&BUFFER_TO_SEND[0]);
  // Close the TCP connection
  ESP8266_SendCommand(__CIPCLOSE);
  Exti0=0;
  // Wait for 15 seconds before the next operation
  HAL_Delay(15000);

}


// Function to prepare the ESP8266 to receive data
void ESP8266_Receive (void)
{
  // Reset the data ready flag
  UART_DMA_DataIsReady = 0;
  // Clear the DMA buffer
  memset (DMA_Buffer, '\0', sizeof(DMA_Buffer));
  // Start UART reception with DMA, receiving data until idle
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)DMA_Buffer, sizeof(DMA_Buffer));
}

// Function to connect the ESP8266 to a WiFi network
void ESP8266_Connect_To_WiFi (uint8_t * wifi_name, uint8_t * password)
{
  uint8_t wifi_Command[40];
  // Format the WiFi connection command with the provided SSID and password
  sprintf((char*)wifi_Command, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_name, password);
  // Send the WiFi connection command to the ESP8266
  ESP8266_SendCommand(wifi_Command);
}

// Function to send a command to the ESP8266 and receive the response
void  ESP8266_SendCommand (uint8_t * command)
{
  // Toggle a GPIO pin (for debugging or indication purposes)
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
  // Prepare to receive the response
  ESP8266_Receive();
  // Transmit the command to the ESP8266
  HAL_UART_Transmit(&huart2, command, strlen((const char*)command), 0xFFFF);
  // Short delay to allow the command to be processed
  HAL_Delay(50);
  // Wait until the data is ready
  while (UART_DMA_DataIsReady == 0) {}
}

// Callback function for UART receive event
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  // Toggle a GPIO pin (for debugging or indication purposes)
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
  // Clear the CPU buffer
  memset (CPU_Buffer, '\0', sizeof(CPU_Buffer));
  // Copy the received data from the DMA buffer to the CPU buffer
  memcpy (CPU_Buffer, DMA_Buffer, (DMAMAXCOUNTER - DMA1_Stream5->NDTR));
  // Prepare to receive the next data
  ESP8266_Receive();
  // Set the data ready flag
  UART_DMA_DataIsReady = 1;
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
