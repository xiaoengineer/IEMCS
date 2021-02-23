#ifndef __IO_DEVICE_H__
#define __IO_DEVICE_H__

#include "stm32f1xx.h"
#include "IEMCS.h"


#define IO0 GPIO_PIN_8
#define IO1 GPIO_PIN_9
#define IO2 GPIO_PIN_10
#define IO3 GPIO_PIN_11

void IO_device_init(void);
void IO_on(uint16_t IO_x);
void IO_off(uint16_t IO_x);

#endif

