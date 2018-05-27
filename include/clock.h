/* Includes */
#include "stm32f1xx.h"

/* Exported types */
/*
* @brief  enumeration for clock modes
  */
typedef enum
{
	MODE_TIME_CLOCK,
	MODE_TIME_SET_HOUR,
	MODE_TIME_SET_MINUTE,
	MODE_TIME_SET_COLOR,
	MODE_TIME_SET_EFFECT,
}Wordclock_Mode;

/* Exported constants */

/* Exported macro */

/* Exported structs */
typedef struct {
   uint16_t x;
   uint16_t y;
   uint16_t width;
} Clock_Term;

/* Exported functions */
void init_clock(void);
void show_time(uint8_t red, uint8_t green, uint8_t blue);
void led_clock_hour_plus(void);
void led_clock_hour_minus(void);
void led_clock_minute_plus(void);
void led_clock_minute_minus(void);
void show_fragment(Clock_Term term, uint8_t red, uint8_t green, uint8_t blue);
void RTC_CalendarConfig(void);
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);
void show_mode_feedback(uint16_t index, uint8_t mode, uint8_t red, uint8_t green, uint8_t blue);



