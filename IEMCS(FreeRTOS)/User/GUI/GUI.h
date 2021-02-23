#ifndef __GUI_H__
#define __GUI_H__

#include "stm32f1xx.h"
#include "oled.h"
#include "font.h"

#define MY_DISPLAY_X_WIDTH  X_WIDTH
#define MY_DISPLAY_Y_WIDTH  Y_WIDTH

#define FONT_LEFT   0
#define FONT_MID    1
#define FONT_RIGHT  2

#define FRAME    1
#define NO_FRANE 0

#define NO_ANTI  0
#define ANTI     1

//typedef struct{
//	uint8_t align;
//	uint8_t frame;
//	uint8_t Anti;

//}text_style_Typedef;


void MyGui_DrawPos(uint8_t x, uint8_t y, uint8_t color);
void MyGUI_DrawLine(uint8_t begin_x,uint8_t begin_y,uint8_t end_x,uint8_t end_y,uint8_t color);
void MyGUI_DrawRec(uint8_t begin_x,uint8_t begin_y,uint8_t end_x,uint8_t end_y);
void MyGUI_Char(uint8_t x,uint8_t y, char* ch,uint8_t align);
void MyGUI_String(uint8_t x,uint8_t y, uint8_t align,char * str);
//void MyGUI_Text(uint8_t x,uint8_t y,uint8_t width,text_style_Typedef style,char * str);

#endif
