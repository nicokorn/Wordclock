/*
 * Autor: Nico Korn
 * Date: 26.10.2017
 * Firmware for the STM32F103 Microcontroller to work with WS2812b leds.
 *  *
 * Copyright (c) 2017 Nico Korn
 *
 * clock_ws2812.c this module contents the time function for the wordclock
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

#include "clock.h"
#include "ws2812.h"
#include "stm32f1xx.h"

/* variables */
static RTC_HandleTypeDef 	RTC_Handle;
static RTC_TimeTypeDef 		timestructureset;
static RTC_DateTypeDef 		datestructureset;
static RTC_TimeTypeDef 		timestructureget;
static RTC_DateTypeDef 		datestructureget;
static Clock_Term			term_es;
static Clock_Term			term_ist;
static Clock_Term			term_fuenf;
static Clock_Term			term_zehn;
static Clock_Term			term_zwanzig;
static Clock_Term			term_drei;
static Clock_Term			term_dreiviertel;
static Clock_Term			term_viertel;
static Clock_Term			term_nach;
static Clock_Term			term_vor;
static Clock_Term			term_halb;
static Clock_Term			term_zwoelf;
static Clock_Term			term_zwei;
static Clock_Term			term_eins;
static Clock_Term			term_sieben;
static Clock_Term			term_drei2;
static Clock_Term			term_fuenf2;
static Clock_Term			term_elf;
static Clock_Term			term_neun;
static Clock_Term			term_vier;
static Clock_Term			term_acht;
static Clock_Term			term_zehn2;
static Clock_Term			term_sechs;
static Clock_Term			term_uhr;
static Clock_Term			term_ecke1;
static Clock_Term			term_ecke2;
static Clock_Term			term_ecke3;
static Clock_Term			term_ecke4;
static Clock_Term			term_ecke_all;

/**
  * @brief  initialization of words & rtc
  * @note   None
  * @retval None
  */
void init_clock(){
	/* init clock terms */
	/* term: "ES" */
	term_es.x = 0;
	term_es.y = 0;
	term_es.width = 2;
	/* term: "IST" */
	term_ist.x = 3;
	term_ist.y = 0;
	term_ist.width = 3;
	/* term: "FÜNF" */
	term_fuenf.x = 7;
	term_fuenf.y = 0;
	term_fuenf.width = 4;
	/* term: "ZEHN" */
	term_zehn.x = 0;
	term_zehn.y = 1;
	term_zehn.width = 4;
	/* term: "ZWANZIG" */
	term_zwanzig.x = 4;
	term_zwanzig.y = 1;
	term_zwanzig.width = 8;
	/* term: "DREI" */
	term_drei.x = 0;
	term_drei.y = 2;
	term_drei.width = 4;
	/* term: "DREIVIERTEL" */
	term_dreiviertel.x = 0;
	term_dreiviertel.y = 2;
	term_dreiviertel.width = 11;
	/* term: "VIERTEL" */
	term_viertel.x = 4;
	term_viertel.y = 2;
	term_viertel.width = 7;
	/* term: "NACH" */
	term_nach.x = 2;
	term_nach.y = 3;
	term_nach.width = 4;
	/* term: "VOR" */
	term_vor.x = 6;
	term_vor.y = 3;
	term_vor.width = 3;
	/* term: "HALB" */
	term_halb.x = 0;
	term_halb.y = 4;
	term_halb.width = 4;
	/* term: "ZWÖLF" */
	term_zwoelf.x = 5;
	term_zwoelf.y = 4;
	term_zwoelf.width = 5;
	/* term: "ZWEI" */
	term_zwei.x = 0;
	term_zwei.y = 5;
	term_zwei.width = 4;
	/* term: "EINS" */
	term_eins.x = 2;
	term_eins.y = 5;
	term_eins.width = 4;
	/* term: "SIEBEN" */
	term_sieben.x = 5;
	term_sieben.y = 5;
	term_sieben.width = 6;
	/* term: "DREI" */
	term_drei2.x = 1;
	term_drei2.y = 6;
	term_drei2.width = 4;
	/* term: "FÜNF" */
	term_fuenf2.x = 7;
	term_fuenf2.y = 6;
	term_fuenf2.width = 11;
	/* term: "ELF" */
	term_elf.x = 0;
	term_elf.y = 7;
	term_elf.width = 3;
	/* term: "NEUN" */
	term_neun.x = 3;
	term_neun.y = 7;
	term_neun.width = 4;
	/* term: "VIER" */
	term_vier.x = 7;
	term_vier.y = 7;
	term_vier.width = 4;
	/* term: "ACHT" */
	term_acht.x = 1;
	term_acht.y = 8;
	term_acht.width = 4;
	/* term: "ZEHN2" */
	term_zehn2.x = 5;
	term_zehn2.y = 8;
	term_zehn2.width = 4;
	/* term: "SECHS" */
	term_sechs.x = 1;
	term_sechs.y = 9;
	term_sechs.width = 5;
	/* term: "UHR" */
	term_uhr.x = 8;
	term_uhr.y = 9;
	term_uhr.width = 3;
	/* term: "ecke1" */
	term_ecke1.x = 3;
	term_ecke1.y = 10;
	term_ecke1.width = 1;
	/* term: "ecke2" */
	term_ecke2.x = 2;
	term_ecke2.y = 10;
	term_ecke2.width = 1;
	/* term: "ecke3" */
	term_ecke3.x = 1;
	term_ecke3.y = 10;
	term_ecke3.width = 1;
	/* term: "ecke4" */
	term_ecke4.x = 0;
	term_ecke4.y = 10;
	term_ecke4.width = 1;
	/* term: "ecke_all" */
	term_ecke_all.x = 0;
	term_ecke_all.y = 10;
	term_ecke_all.width = 4;

	/* Configure RTC prescaler and RTC data registers */
	/* RTC configured as follow:
	- Asynch Prediv  = Calculated automatically by HAL */
	RTC_Handle.Instance = 			RTC;
	RTC_Handle.Init.AsynchPrediv = 	RTC_AUTO_1_SECOND;

	if (HAL_RTC_Init(&RTC_Handle) != HAL_OK){
	    /* Initialization Error */
	    //Error_Handler();
	}

	/* Read the Back Up Register 1 Data */
	if (HAL_RTCEx_BKUPRead(&RTC_Handle, RTC_BKP_DR1) != 0x32F2){
	    /* Configure RTC Calendar */
	    RTC_CalendarConfig();
	}else{
		/* Clear source Reset Flag */
	    __HAL_RCC_CLEAR_RESET_FLAGS();
	}
}

/**
  * @brief  draws a fragment into the IO buffer
  * @note   None
  * @retval None
  */
void show_fragment(Clock_Term term, uint8_t red, uint8_t green, uint8_t blue){
	for(int x = 0; x < term.width; x++){
		WS2812_framedata_setPixel((uint8_t)term.y, (uint16_t)term.x+x, (uint8_t)red, (uint8_t)green, (uint8_t)blue);
	}
}

/**
  * @brief  reads time from rtc and draw fragments into the frame buffer
  * @note   None
  * @retval None
  */
void show_time(uint8_t red, uint8_t green, uint8_t blue){
	RTC_TimeTypeDef timestructureget;
	RTC_DateTypeDef datestructureget;
	HAL_RTC_GetTime(&RTC_Handle, &timestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handle, &datestructureget, RTC_FORMAT_BIN);

	/* hours and minutes local places: hh:mm = h0h1:m0m2 */
	uint16_t hours = 	timestructureget.Hours;
	uint16_t minutes = 	timestructureget.Minutes;
	uint16_t h12 = 		0;
	uint16_t m1 = 		0;

	/* process hour */
	h12 = hours % 12;

	/* process minutes */
	m1 = minutes % 10;

	/* prepare numbers for display */
	switch(h12){
		case 0:		if(minutes < 25){
						show_fragment(term_zwoelf, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_eins, red, green, blue);
					}
		break;
		case 1:		if(minutes < 25){
						show_fragment(term_eins, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_zwei, red, green, blue);
					}
		break;
		case 2:		if(minutes < 25){
						show_fragment(term_zwei, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_drei2, red, green, blue);
					}
		break;
		case 3:		if(minutes < 25){
						show_fragment(term_drei2, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_vier, red, green, blue);
					}
		break;
		case 4:		if(minutes < 25){
						show_fragment(term_vier, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_fuenf2, red, green, blue);
					}
		break;
		case 5:		if(minutes < 25){
						show_fragment(term_fuenf2, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_sechs, red, green, blue);
					}
		break;
		case 6:		if(minutes < 25){
						show_fragment(term_sechs, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_sieben, red, green, blue);
					}
		break;
		case 7:		if(minutes < 25){
						show_fragment(term_sieben, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_acht, red, green, blue);
					}
		break;
		case 8:		if(minutes < 25){
						show_fragment(term_acht, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_neun, red, green, blue);
					}
		break;
		case 9:		if(minutes < 25){
						show_fragment(term_neun, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_zehn2, red, green, blue);
					}
		break;
		case 10:	if(minutes < 25){
						show_fragment(term_zehn2, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_elf, red, green, blue);
					}
		break;
		case 11:	if(minutes < 25){
						show_fragment(term_elf, red, green, blue);
					}else if(minutes >= 25){
						show_fragment(term_zwoelf, red, green, blue);
					}
		break;
	}

	if(minutes == 0){
		show_fragment(term_uhr, red, green, blue);
	}

	switch(minutes){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:		show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_uhr, red, green, blue);
		break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:		show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_fuenf, red, green, blue);
					show_fragment(term_nach, red, green, blue);
		break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_zehn, red, green, blue);
					show_fragment(term_nach, red, green, blue);
		break;
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_viertel, red, green, blue);
					show_fragment(term_nach, red, green, blue);
		break;
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_zwanzig, red, green, blue);
					show_fragment(term_nach, red, green, blue);
		break;
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_fuenf, red, green, blue);
					show_fragment(term_vor, red, green, blue);
					show_fragment(term_halb, red, green, blue);
		break;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_halb, red, green, blue);
		break;
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_fuenf, red, green, blue);
					show_fragment(term_nach, red, green, blue);
					show_fragment(term_halb, red, green, blue);
		break;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_zwanzig, red, green, blue);
					show_fragment(term_vor, red, green, blue);
		break;
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_viertel, red, green, blue);
					show_fragment(term_vor, red, green, blue);
		break;
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_zehn, red, green, blue);
					show_fragment(term_vor, red, green, blue);
		break;
		case 55:
		case 56:
		case 57:
		case 58:
		case 59:	show_fragment(term_es, red, green, blue);
					show_fragment(term_ist, red, green, blue);
					show_fragment(term_fuenf, red, green, blue);
					show_fragment(term_vor, red, green, blue);
		break;
	}

	switch(m1){
		case 0:;
		break;
		case 1:		show_fragment(term_ecke1, red, green, blue);
		break;
		case 2:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
		break;
		case 3:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
					show_fragment(term_ecke3, red, green, blue);
		break;
		case 4:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
					show_fragment(term_ecke3, red, green, blue);
					show_fragment(term_ecke4, red, green, blue);
		break;
		case 5:;
		break;
		case 6:		show_fragment(term_ecke1, red, green, blue);
		break;
		case 7:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
		break;
		case 8:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
					show_fragment(term_ecke3, red, green, blue);
		break;
		case 9:		show_fragment(term_ecke1, red, green, blue);
					show_fragment(term_ecke2, red, green, blue);
					show_fragment(term_ecke3, red, green, blue);
					show_fragment(term_ecke4, red, green, blue);
		break;
	}
}

/**
  * @brief  set hour +1 on the RTC registers
  * @note   None
  * @retval None
  */
void led_clock_hour_plus(){
	HAL_RTC_GetTime(&RTC_Handle, &timestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handle, &datestructureget, RTC_FORMAT_BIN);

	if(timestructureget.Hours + 0x01 < 0x18){
		timestructureset.Hours = timestructureget.Hours + 0x01;
	}else{
		timestructureset.Hours = 0x00;
	}
	timestructureset.Minutes = 	timestructureget.Minutes;
	timestructureset.Seconds = 	timestructureget.Seconds;

	datestructureset.Year = 	datestructureget.Year;
	datestructureset.Month = 	datestructureget.Month;
	datestructureset.Date = 	datestructureget.Date;
	datestructureset.WeekDay = 	datestructureget.WeekDay;

	HAL_RTC_SetTime(&RTC_Handle, &timestructureset, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&RTC_Handle, &datestructureset, RTC_FORMAT_BIN);

}

/**
  * @brief  set hour -1 on the RTC registers
  * @note   None
  * @retval None
  */
void led_clock_hour_minus(){
	HAL_RTC_GetTime(&RTC_Handle, &timestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handle, &datestructureget, RTC_FORMAT_BIN);

	if(timestructureget.Hours - 0x01 > 0x00){
		timestructureset.Hours = timestructureget.Hours - 0x01;
	}else{
		timestructureset.Hours = 0x17;
	}
	timestructureset.Minutes = 	timestructureget.Minutes;
	timestructureset.Seconds = 	timestructureget.Seconds;

	datestructureset.Year = 	datestructureget.Year;
	datestructureset.Month = 	datestructureget.Month;
	datestructureset.Date = 	datestructureget.Date;
	datestructureset.WeekDay = 	datestructureget.WeekDay;

	HAL_RTC_SetTime(&RTC_Handle, &timestructureset, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&RTC_Handle, &datestructureset, RTC_FORMAT_BIN);
}

/**
  * @brief  set minute +1 on the RTC registers
  * @note   None
  * @retval None
  */
void led_clock_minute_plus(){
	HAL_RTC_GetTime(&RTC_Handle, &timestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handle, &datestructureget, RTC_FORMAT_BIN);

	timestructureset.Hours = timestructureget.Hours;
	if(timestructureget.Minutes + 0x01 < 0x3c){
		timestructureset.Minutes = timestructureget.Minutes + 0x01;
	}else{
		timestructureset.Minutes = 0x00;
	}
	timestructureset.Seconds = timestructureget.Seconds;

	datestructureset.Year = 	datestructureget.Year;
	datestructureset.Month = 	datestructureget.Month;
	datestructureset.Date = 	datestructureget.Date;
	datestructureset.WeekDay = 	datestructureget.WeekDay;

	HAL_RTC_SetTime(&RTC_Handle, &timestructureset, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&RTC_Handle, &datestructureset, RTC_FORMAT_BIN);
}

/**
  * @brief  set minute -1 on the RTC registers
  * @note   None
  * @retval None
  */
void led_clock_minute_minus(){
	HAL_RTC_GetTime(&RTC_Handle, &timestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTC_Handle, &datestructureget, RTC_FORMAT_BIN);

	timestructureset.Hours = timestructureget.Hours;
	if(timestructureget.Minutes - 0x01 > 0x00){
		timestructureset.Minutes = timestructureget.Minutes - 0x01;
	}else{
		timestructureset.Minutes = 0x3b;
	}
	timestructureset.Seconds = timestructureget.Seconds;

	datestructureset.Year = 	datestructureget.Year;
	datestructureset.Month = 	datestructureget.Month;
	datestructureset.Date = 	datestructureget.Date;
	datestructureset.WeekDay = 	datestructureget.WeekDay;

	HAL_RTC_SetTime(&RTC_Handle, &timestructureset, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&RTC_Handle, &datestructureset, RTC_FORMAT_BIN);
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarConfig(void){
	/*##-1- Configure the Date #################################################*/
	/* Set Date: Tuesday February 18th 2014 */
	datestructureset.Year = 	0x14;
	datestructureset.Month = 	RTC_MONTH_FEBRUARY;
	datestructureset.Date = 	0x18;
	datestructureset.WeekDay = 	RTC_WEEKDAY_TUESDAY;

	if(HAL_RTC_SetDate(&RTC_Handle,&datestructureset,RTC_FORMAT_BCD) != HAL_OK){
		/* Initialization Error */
		//Error_Handler();
	}

	/*##-2- Configure the Time #################################################*/
	/* Set Time: 02:00:00 */
	timestructureset.Hours = 	0x02;
	timestructureset.Minutes = 	0x00;
	timestructureset.Seconds = 	0x00;

	if (HAL_RTC_SetTime(&RTC_Handle, &timestructureset, RTC_FORMAT_BCD) != HAL_OK){
		/* Initialization Error */
		//Error_Handler();
	}

	/*##-3- Writes a data in a RTC Backup data Register1 #######################*/
	HAL_RTCEx_BKUPWrite(&RTC_Handle, RTC_BKP_DR1, 0x32F2);
}

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc){
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	/*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
	/* To change the source clock of the RTC feature (LSE, LSI), You have to:
     	 - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     	 - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
       	   configure the RTC clock source (to be done once after reset).
     	 - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
       	   __HAL_RCC_BACKUPRESET_RELEASE().
     	 - Configure the needed RTc clock source */
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	/* Enable BKP CLK for backup registers */
	__HAL_RCC_BKP_CLK_ENABLE();

	/*##-2- Configue LSE as RTC clock soucre ###################################*/
	RCC_OscInitStruct.OscillatorType =	RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = 	RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = 		RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
		//Error_Handler();
	}

	PeriphClkInitStruct.PeriphClockSelection = 	RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = 	RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK){
		//Error_Handler();
	}

	/*##-3- Enable RTC peripheral Clocks #######################################*/
	/* Enable RTC Clock */
	__HAL_RCC_RTC_ENABLE();
}

/**
  * @brief  mode feedback effect
  * @note   draws mode feedback effects into the IO Buffer
  * @retval None
  */
void show_mode_feedback(uint16_t index, uint8_t mode, uint8_t red, uint8_t green, uint8_t blue){
	switch(mode){
		case MODE_TIME_CLOCK:		for(uint16_t x = 0; x<11; x++){
										for(uint8_t y = 0; y<11; y++){
											WS2812_framedata_setPixel(y, x, red, green, blue);
										}
									}
		break;
		case MODE_TIME_SET_HOUR:	for(uint16_t x = 0; x<11; x++){
										for(uint8_t y = 5; y<11; y++){
											WS2812_framedata_setPixel(y, x, red, green, blue);
										}
									}
		break;
		case MODE_TIME_SET_MINUTE:	for(uint16_t x = 0; x<11; x++){
										for(uint8_t y = 0; y<5; y++){
											WS2812_framedata_setPixel(y, x, red, green, blue);
										}
									}
		break;
		case MODE_TIME_SET_COLOR:		if(index > 0 && index < 5){
											for(uint16_t x = 0; x<11; x++){
												for(uint8_t y = 0; y<11; y++){
													WS2812_framedata_setPixel(y, x, 0xff, 0x00, 0x00);
												}
											}
										}else if(index > 5 && index < 10){
											for(uint16_t x = 0; x<11; x++){
												for(uint8_t y = 0; y<11; y++){
													WS2812_framedata_setPixel(y, x, 0x00, 0xff, 0x00);
												}
											}
										}else if(index > 10 && index < 15){
											for(uint16_t x = 0; x<11; x++){
												for(uint8_t y = 0; y<11; y++){
													WS2812_framedata_setPixel(y, x, 0x00, 0x00, 0xff);
												}
											}
										}else if(index > 15 && index < 20){
											for(uint16_t x = 0; x<11; x++){
												for(uint8_t y = 0; y<11; y++){
													WS2812_framedata_setPixel(y, x, 0xff, 0x00, 0x00);
												}
											}
										}
		break;
		case MODE_TIME_SET_EFFECT:		//WS2812_clear_buffer();
										/* letter F */
										WS2812_set_line(9,0,0,0,0x00,0x00,0xff);
										WS2812_set_line(0,0,0,4,0x00,0x00,0xff);
										WS2812_set_line(4,0,4,3,0x00,0x00,0xff);
										/* letter X */
										WS2812_set_line(0,6,9,10,0x00,0x00,0xff);
										WS2812_set_line(9,6,0,10,0x00,0x00,0xff);
		break;
	}
}
