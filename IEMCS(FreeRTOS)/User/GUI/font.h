#ifndef __FONT_H__
#define __FONT_H__

#include "stm32f1xx.h"

extern uint8_t GetFontASCII_5_7(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontASCII_6_12(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontGb2312_12_12(uint8_t codeH, uint8_t codeL, uint8_t *pBuff);
#endif
