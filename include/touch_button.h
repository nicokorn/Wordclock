/* Includes */
#include "stm32f1xx.h"

/* Exported defines */
#define BUTTON_LEFT 0x4000
#define BUTTON_MIDDLE 0x2000
#define BUTTON_RIGHT 0x1000

/* Exported constants */

/* Exported macro */

/* Exported functions */
void init_touch_buttons(void);
void show_button_feedback(uint16_t index, uint16_t button);
