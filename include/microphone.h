/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MICROPHONE_H
#define __MICROPHONE_H

/* Includes */
#include "ws2812.h"
#include "stm32f1xx.h"

/* Exported types */
/*
* @brief  enumeration for touchbuttons
  */

/* Exported constants */

/* Exported macro */

/* Exported functions */
void init_microphone(void);
void init_gpio_microphone(void);
void init_adc_microphone(void);
void start_microphone_adc_conversion(void);
void stop_microphone_adc_conversion(void);
void get_microphone_adc_conversion(uint32_t *adc_conversion);

#endif /* __MICROPHONE_H */
