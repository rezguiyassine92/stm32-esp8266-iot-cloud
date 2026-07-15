/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_adc1; // Ajout du DMA pour l'ADC

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

void NMI_Handler(void) { while (1) {} }

void HardFault_Handler(void) { while (1) {} }

void MemManage_Handler(void) { while (1) {} }

void BusFault_Handler(void) { while (1) {} }

void UsageFault_Handler(void) { while (1) {} }

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/******************************************************************************/

/**
  * @brief Interruption du bouton PA0
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
  * @brief DMA de l'UART2 (Réception ESP8266)
  */
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/**
  * @brief DMA de l'ADC1 (Nouveau : gestion des 3 canaux)
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* Ce gestionnaire est crucial pour que le callback HAL_ADC_ConvCpltCallback
     soit appelé dans ton main.c */
  HAL_DMA_IRQHandler(&hdma_adc1);
}

/**
  * @brief Interruption globale ADC
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}

/**
  * @brief Interruption globale USART2
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
}
