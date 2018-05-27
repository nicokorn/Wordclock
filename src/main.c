/*
 * Autor: Nico Korn
 * Date: 04.02.2018
 * Firmware for the STM32F103 Microcontroller to work with WS2812b leds.
 *  *
 * Copyright (c) 2018 Nico Korn
 *
 * main.c this module contents the main routine with fsm
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* defines */
#define CLOCK_REFRESH_PERIOD 	1
#define FEEDBACK_REFRESH_PERIOD 50
#define FEEDBACK_FRAMES 		13
#define MODE_STANDBY_COUNT 		10000 //ms
/* variables */
extern uint8_t 		WS2812_TC;
static uint8_t 		mode;
static uint8_t 		fx_mode;
static uint8_t 		red,green,blue;
static uint16_t 	buttons;
static uint32_t 	system_counter;
/* function prototypes */
void SystemClock_Config(void);
void refresh_clock(void);
void show_mode(uint8_t *mode, uint8_t *red, uint8_t *green, uint8_t *blue);
void show_button(uint8_t *fx_mode, uint16_t *buttons, uint8_t *red, uint8_t *green, uint8_t *blue);
void get_preferences(uint8_t *fx_mode, uint8_t *red, uint8_t *green, uint8_t *blue);
void set_preferences(uint8_t *fx_mode, uint8_t *red, uint8_t *green, uint8_t *blue);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){

	/* STM32F103xG HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
    */
	HAL_Init();

	/* Configure the system clock to 72 MHz */
	SystemClock_Config();

	/* initialize ws2812 peripherals */
	init_ws2812();

	/* initialize touch buttons */
	init_touch_buttons();

	/* initialize microphone */
	init_microphone();

	/* load color and fx mode preferences from the BKP register */
	get_preferences(&fx_mode, &red, &green, &blue);

	/* initialize clock (also load time information from BKP register) */
	init_clock();

	/* init system counter & mode */
	system_counter = 0;
	mode = MODE_TIME_CLOCK;

	/* test leds */
	WS2812_led_test();

	while (1){
		/* clear all touch buttons */
		buttons = 0x0000;
		/* read touch buttons: 0x4000 = left, 0x2000 = middle, 0x1000 = right */
		buttons = (uint16_t)GPIOB->IDR & 0x7000;
		/* set system counter to 0 if a button has been set
		 * the counter is used to automatically go back into the clock mode
		 * after MODE_STANDBY_COUNT time and is used to set the refresh rate
		 * of the background effects
		 * */
		if(buttons != RESET){
			system_counter = 0;
		}

		/* finite state machine */
		switch(mode){
			case MODE_TIME_CLOCK: 		switch(buttons){
											case BUTTON_LEFT:	/* do nothing */;
											break;
											case BUTTON_MIDDLE:	/* set next mode */
																mode = MODE_TIME_SET_HOUR;
																/* show feedback effects */
																show_mode(&mode, &red, &green, &blue);
											break;
											case BUTTON_RIGHT:	/* do nothing */;
											break;
											default:			/* stay in this mode */
																mode = MODE_TIME_CLOCK;
										}
			break;
			case MODE_TIME_SET_HOUR: 	switch(buttons){
											case BUTTON_LEFT:	/* increment 1 hour */
																led_clock_hour_plus();
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											case BUTTON_MIDDLE:	/* set next mode */
																mode = MODE_TIME_SET_MINUTE;
																/* show feedback effects */
																show_mode(&mode, &red, &green, &blue);
											break;
											case BUTTON_RIGHT:	/* decrement 1 hour */
																led_clock_hour_minus();
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											default:			/* stay in this mode for certain time, then go back in clock mode*/
																if(system_counter > MODE_STANDBY_COUNT){
																	mode = MODE_TIME_CLOCK;
																	/* show feedback effects */
																	show_mode(&mode, &red, &green, &blue);
																}else{
																	mode = MODE_TIME_SET_HOUR;
																}
											}
			break;
			case MODE_TIME_SET_MINUTE: 	switch(buttons){
											case BUTTON_LEFT:	/* increment 1 minute */
																led_clock_minute_plus();
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											case BUTTON_MIDDLE:	/* set next mode */
																mode = MODE_TIME_SET_COLOR;
																/* show feedback effects */
																show_mode(&mode, &red, &green, &blue);
											break;
											case BUTTON_RIGHT:	/* decrement 1 minute */
																led_clock_minute_minus();
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											default:			/* stay in this mode for certain time, then go back in clock mode*/
																if(system_counter > MODE_STANDBY_COUNT){
																	mode = MODE_TIME_CLOCK;
																	/* show feedback effects */
																	show_mode(&mode, &red, &green, &blue);
																}else{
																	mode = MODE_TIME_SET_MINUTE;
																}
										}
			break;
			case MODE_TIME_SET_COLOR: 	switch(buttons){
											case BUTTON_LEFT:	/* increment color */
																for(uint16_t i=0; i<40; i++){
																	WS2812_color_wheel_plus(&red,&green,&blue);
																}
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											case BUTTON_MIDDLE:	/* set next mode */
																mode = MODE_TIME_SET_EFFECT;
																/* show feedback effects */
																show_mode(&mode, &red, &green, &blue);
											break;
											case BUTTON_RIGHT:	/* decrement color */
																for(uint16_t i=0; i<40; i++){
																	WS2812_color_wheel_minus(&red,&green,&blue);
																}
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											default:			/* stay in this mode for certain time, then go back in clock mode*/
																if(system_counter > MODE_STANDBY_COUNT){
																	mode = MODE_TIME_CLOCK;
																	/* show feedback effects */
																	show_mode(&mode, &red, &green, &blue);
																}else{
																	mode = MODE_TIME_SET_COLOR;
																}
											}
			break;
			case MODE_TIME_SET_EFFECT: 	switch(buttons){
											case BUTTON_LEFT:	/* choose next background */
																if(fx_mode < 3){
																	fx_mode++;
																}else{
																	fx_mode = 0;
																}
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											case BUTTON_MIDDLE:	/* set next mode */
																mode = MODE_TIME_CLOCK;
																/* show feedback effects */
																show_mode(&mode, &red, &green, &blue);
											break;
											case BUTTON_RIGHT:	/* choose last background */
																if(fx_mode > 0){
																	fx_mode--;
																}else{
																	fx_mode = 3;
																}
																/* show feedback effects and keep background running */
																show_button(&fx_mode, &buttons, &red, &green, &blue);
											break;
											default:			/* stay in this mode for certain time, then go back in clock mode*/
																if(system_counter > MODE_STANDBY_COUNT){
																	mode = MODE_TIME_CLOCK;
																	/* show feedback effects */
																	show_mode(&mode, &red, &green, &blue);
																}else{
																	mode = MODE_TIME_SET_EFFECT;
																}
											}
			break;
		}

		/* refresh clock */
		refresh_clock();
	}
}

/**
  * @brief  this function is used to refresh time and background of the wordclock
  * @param  None
  * @retval None
  */
void refresh_clock(){
	/* set background */
	WS2812_set_clock_fx(&fx_mode, &system_counter, &red, &green, &blue);
	/* write time into frame buffer */
	show_time(red, green, blue);
	/* safe settings into BKP registers */
	set_preferences(&fx_mode, &red, &green, &blue);
	/* wait for the data transmission to the led's to be ready */
	while(!WS2812_TC);
	/* send frame buffer to the leds */
	sendbuf_WS2812();
	/* synchronised system counter with hal_delay */
	HAL_Delay(CLOCK_REFRESH_PERIOD);
	system_counter++;
}

/**
  * @brief  this function is used to show mode feedback effect
  * @param  None
  * @retval None
  */
void show_mode(uint8_t *mode, uint8_t *red, uint8_t *green, uint8_t *blue){
	/* show feedback effects */
	for(uint16_t i=0; i<FEEDBACK_FRAMES; i++){
		if(i%2){
			/* show feedback effect */
			show_mode_feedback(i, *mode, *red, *green, *blue);
		}else{
			/* dark frame */
			WS2812_clear_buffer();
		}
		/* wait for the data transmission to the led's to be ready */
		while(!WS2812_TC);
		/* send frame buffer to the leds */
		sendbuf_WS2812();
		/* delay */
		HAL_Delay(FEEDBACK_REFRESH_PERIOD);
	}
}

/**
  * @brief  this function is used to show button feedback effect
  * @param  None
  * @retval None
  */
void show_button(uint8_t *fx_mode, uint16_t *buttons, uint8_t *red, uint8_t *green, uint8_t *blue){
	uint32_t dummy = 0;
	/* show feedback effects and keep background running */
	for(uint16_t i=0; i<FEEDBACK_FRAMES; i++){
		/* set background */
		WS2812_set_clock_fx(fx_mode, &dummy, red, green, blue);
		/* write time into frame buffer */
		show_time(*red, *green, *blue);
		/* show feedback effect */
		show_button_feedback(i, *buttons);
		/* wait for the data transmission to the led's to be ready */
		while(!WS2812_TC);
		/* send frame buffer to the leds */
		sendbuf_WS2812();
		/* delay */
		HAL_Delay(FEEDBACK_REFRESH_PERIOD);
	}
}

/**
  * @brief  this function is used to get color and mode preferences from the backup register BKP_DR
  * @param  None
  * @retval None
  */
void get_preferences(uint8_t *fx_mode, uint8_t *red, uint8_t *green, uint8_t *blue){
	  uint32_t backupregister = 0U;
	  uint32_t backupregister_value = 0U;
	  uint32_t backup_register_mask = 0x000000FF;

	  /* get reset variable */
	  backupregister = (uint32_t)BKP_BASE;
	  backupregister += (1 * 4U);
	  backupregister_value = (*(__IO uint32_t *)(backupregister)) & BKP_DR1_D;

	  /* if variable = 0x32F2, BKP registers have saved preferences */
	  if(backupregister_value != 0x32F2){
			/* init default color */
			*red = 0x00;
			*green = 0x00;
			*blue = 0xff;
			/* init default fx mode */
			*fx_mode = 0;
	  }else{
		  /* get red color */
		  backupregister = (uint32_t)BKP_BASE;
		  backupregister += (2 * 4U);

		  *red = (*(__IO uint32_t *)(backupregister)) & (uint32_t)backup_register_mask;
		  backupregister = 0U;

		  /* get green color */
		  backupregister = (uint32_t)BKP_BASE;
		  backupregister += (3 * 4U);

		  *green = (*(__IO uint32_t *)(backupregister)) & (uint32_t)backup_register_mask;
		  backupregister = 0U;


		  /* get blue color */
		  backupregister = (uint32_t)BKP_BASE;
		  backupregister += (4 * 4U);

		  *blue = (*(__IO uint32_t *)(backupregister)) & (uint32_t)backup_register_mask;
		  backupregister = 0U;


		  /* get fx mode */
		  backupregister = (uint32_t)BKP_BASE;
		  backupregister += (5 * 4U);

		  *fx_mode = (*(__IO uint32_t *)(backupregister)) & (uint32_t)backup_register_mask;
		  backupregister = 0U;
	  }
}

/**
  * @brief  this function is used to set color and mode preferences from the backup register BKP_DR
  * @param  None
  * @retval None
  */
void set_preferences(uint8_t *fx_mode, uint8_t *red, uint8_t *green, uint8_t *blue){
	  uint32_t tmp = 0U;
	  uint32_t backup_register_mask = 0x000000FF;

	  /* set red color */
	  tmp = (uint32_t)BKP_BASE;
	  tmp += (2 * 4U);

	  *(__IO uint32_t *) tmp = ( *(__IO uint32_t *) red & backup_register_mask);
	  tmp = 0U;

	  /* set green color */
	  tmp = (uint32_t)BKP_BASE;
	  tmp += (3 * 4U);

	  *(__IO uint32_t *) tmp =  ( *(__IO uint32_t *) green & backup_register_mask);
	  tmp = 0U;

	  /* set blue color */
	  tmp = (uint32_t)BKP_BASE;
	  tmp += (4 * 4U);

	  *(__IO uint32_t *) tmp = ( *(__IO uint32_t *) blue & backup_register_mask);
	  tmp = 0U;

	  /* set fx mode color */
	  tmp = (uint32_t)BKP_BASE;
	  tmp += (5 * 4U);

	  *(__IO uint32_t *) tmp = ( *(__IO uint32_t *) fx_mode & backup_register_mask);
	  tmp = 0U;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
