/*
 * Autor: Nico Korn
 * Date: 29.01.2018
 * Firmware for the STM32F103 Microcontroller to work with WS2812b leds.
 *  *
 * Copyright (c) 2017 Nico Korn
 *
 * touch_buttons.c this module contents touch_button init and functions.
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

#include "touch_button.h"
#include "ws2812.h"
#include "stm32f1xx.h"

/**
  * @brief  initialization of touch buttons
  * @note   None
  * @retval None
  */
void init_touch_buttons(){
	/* init button gpio */
	__HAL_RCC_GPIOB_CLK_ENABLE();								//enable clock on the bus
	GPIO_InitTypeDef GPIO_InitStruct_Button;
	GPIO_InitStruct_Button.Pin = 	(uint16_t)0x7000U; 			// select pin 12,13,14 (0x7000U)
	GPIO_InitStruct_Button.Mode = 	GPIO_MODE_IT_FALLING; 		// configure pins for pp o,utput
	GPIO_InitStruct_Button.Speed = 	GPIO_SPEED_FREQ_HIGH;		// 50 MHz rate
	GPIO_InitStruct_Button.Pull = 	GPIO_PULLUP;				// this activates the pullup resistors on the IO pins
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_Button);				// setting GPIO registers
}

/**
  * @brief  button feedback effect
  * @note   None
  * @retval None
  */
void show_button_feedback(uint16_t index, uint16_t button){
	switch(button){
		case BUTTON_LEFT:	if(index < 10){
								WS2812_framedata_setPixel((uint8_t)9, (uint16_t)index, 0xFF, 0xFF, 0xFF);
								WS2812_framedata_setPixel((uint8_t)9-(uint8_t)index, (uint16_t)0, 0xFF, 0xFF, 0xFF);
								WS2812_framedata_setPixel((uint8_t)9-(uint8_t)index, (uint16_t)index, 0xFF, 0xFF, 0xFF);
							}
		break;
		case BUTTON_RIGHT:	if(index < 10){
								WS2812_framedata_setPixel((uint8_t)9, (uint16_t)10-(uint16_t)index, 0xFF, 0xFF, 0xFF);
								WS2812_framedata_setPixel((uint8_t)9-(uint8_t)index, (uint16_t)10, 0xFF, 0xFF, 0xFF);
								WS2812_framedata_setPixel((uint8_t)9-(uint8_t)index, (uint16_t)10-(uint16_t)index, 0xFF, 0xFF, 0xFF);
							}
		break;
	}
}
