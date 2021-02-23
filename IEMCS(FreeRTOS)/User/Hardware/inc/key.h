#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f1xx.h"
#include "IEMCS.h"

void key_gpio_init(void);
void KEY_Scan(void);
uint8_t Get_KeyVal(void);

#endif
