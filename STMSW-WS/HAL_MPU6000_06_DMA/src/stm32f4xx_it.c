/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @date    25/04/2015 20:33:26
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern double tempC;
extern double X_accel, Y_accel, Z_accel;
uint8_t data[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI Line0 interrupt.
* This interrupt is fire by MPU data ready
*/
void EXTI0_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		//CS goes LOW
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		//Put first address to read
		data[0]=MPUREG_ACCEL_XOUT_H|0x80;
		//Receive with DMA 16 byte + address byte
		HAL_SPI_Receive_DMA(&hspi2, data, 17);
	}
}
// DMA Receive Complete ISR Callback
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	//CS goes LOW
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	//Process MPU register data
	int16_t tmp=0;
    tmp=(data[7]<<8) | data[8];
    tempC = (double)(tmp)/340.0 + 36.53;
    X_accel = (int16_t)((data[1]<<8) | data[2])/16384.0;
    Y_accel = (int16_t)((data[3]<<8) | data[4])/16384.0;
    Z_accel = (int16_t)((data[5]<<8) | data[6])/16384.0;
}
/**
* @brief This function handles SPI2 global interrupt.
*/
void SPI2_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi2);
}
/**
* @brief This function handles DMA1 Stream3 global interrupt.
*/
void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
}
/**
* @brief This function handles DMA1 Stream4 global interrupt.
*/
void DMA1_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/