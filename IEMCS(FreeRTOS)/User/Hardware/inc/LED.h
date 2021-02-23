#ifndef __LED_H__
#define __LED_H__

#include "stm32f1xx.h"

#define LED_RED    GPIO_PIN_6
#define LED_GREEN  GPIO_PIN_7


void led_goio_init(void);
void led_on(uint16_t gpio_pin_x);
void led_off(uint16_t gpio_pin_x);
void led_toggle(uint16_t gpio_pin_x);
	
#endif
