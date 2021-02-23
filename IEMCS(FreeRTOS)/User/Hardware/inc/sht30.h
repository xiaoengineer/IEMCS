#ifndef __SHT30_H__
#define __SHT30_H__

#include "stm32f1xx.h"
#include "i2c.h"
#include "IEMCS.h"


void SHT30_init(void);
void read_sht30(void);
#endif
