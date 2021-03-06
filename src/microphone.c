/*
 * Autor: Nico Korn
 * Date: 18.02.2018
 * Firmware for the STM32F103 Microcontroller to work with WS2812b leds.
 *  *
 * Copyright (c) 2017 Nico Korn
 *
 * microphone.c this module contents microphone init and functions.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include "microphone.h"

/* typedefs */
static	RCC_PeriphCLKInitTypeDef  	PeriphClkInit;
static 	ADC_HandleTypeDef 			ADCHandle;
static	ADC_ChannelConfTypeDef 		ADCChConfig;

/**
  * @brief  initialization of microphone
  * @note   None
  * @retval None
  */
void init_microphone(){
	/* init peripherals for microphone */
	init_gpio_microphone();
	init_adc_microphone();
}

/**
  * @brief  initialization of gpio for adc
  * @note   None
  * @retval None
  */
void init_gpio_microphone(){
	/* variables */
	GPIO_InitTypeDef GPIO_InitStruct_Button;
	/* init gpio input pin as analog*/
	__HAL_RCC_GPIOB_CLK_ENABLE();								//enable clock on the bus
	GPIO_InitStruct_Button.Pin = 	(uint16_t)0x0001U; 			// select pin 0
	GPIO_InitStruct_Button.Mode = 	GPIO_MODE_ANALOG; 			// configure pins for pp o,utput
	GPIO_InitStruct_Button.Speed = 	GPIO_SPEED_FREQ_HIGH;		// 50 MHz rate
	GPIO_InitStruct_Button.Pull = 	GPIO_NOPULL;				// this activates the pullup resistors on the IO pins
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_Button);				// setting GPIO registers
}

/**
  * @brief  initialization of gpio for adc
  * @note   None
  * @retval None
  */
void init_adc_microphone(){
	/* Enable and set up clock of ADCx peripheral */
	__HAL_RCC_ADC1_CLK_ENABLE();

	/* configure adc prescaler to meet 12 mhz for the adc clock (14 mhz is the max) */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	/* configure adc */
	ADCHandle.Instance = ADC1;
	ADCHandle.Init.ContinuousConvMode = DISABLE;					// continuous mode disabled to have only 1 conversion at each conversion trig
	ADCHandle.Init.DiscontinuousConvMode = DISABLE;
	ADCHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADCHandle.Init.ScanConvMode = ADC_SCAN_DISABLE;					// single mode, only rank 1
	ADCHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	HAL_ADC_Init(&ADCHandle);

	/* Channel configuration */
	ADCChConfig.Channel = ADC_CHANNEL_8;
	ADCChConfig.Rank = ADC_REGULAR_RANK_1;
	ADCChConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;//ADC_SAMPLETIME_1CYCLE_5;
	HAL_ADC_ConfigChannel(&ADCHandle, &ADCChConfig);

	/* calibration */
	while(HAL_ADCEx_Calibration_Start(&ADCHandle) != HAL_OK);
}

/**
  * @brief  initialization of dma between adc and memory
  * @note   None
  * @retval None
  */
void init_dma_microphone(){
	/* variables */

	/* init gpio input pin as analog*/

}

/**
  * @brief  start analog to digital conversion
  * @note   None
  * @retval None
  */
void start_microphone_adc_conversion(){
	HAL_ADC_Start(&ADCHandle);
	HAL_Delay(1);
}

/**
  * @brief  stop analog to digital conversion
  * @note   None
  * @retval None
  */
void stop_microphone_adc_conversion(){
	HAL_ADC_Stop(&ADCHandle);
}

/**
  * @brief  get recent adc conversion value
  * @note   None
  * @retval None
  */
void get_microphone_adc_conversion(uint32_t *adc_conversion){
	*adc_conversion = HAL_ADC_GetValue(&ADCHandle);
}
