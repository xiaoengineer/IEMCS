#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f1xx.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  

		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

extern uint8_t displayBuff[Y_WIDTH/8][X_WIDTH];


#define USE_STM32_IICDMA

#ifdef USE_STM32_IICDMA
void OLED_Init(void);
void oled_ClearDisplay(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void oled_updataAlldisbuff(void);
void RangeUpdateDisplay(int beginx, int beginy, int endx, int endy);
void oled_set_pos(int x, int y, int color);
int oled_get_pos(int x, int y);

#else
//OLED控制用函数
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void oled_ClearDisplay(void);
void oled_updataAlldisbuff(void);
void RangeUpdateDisplay(int beginx, int beginy, int endx, int endy);
void oled_set_pos(int x, int y, int color);
int oled_get_pos(int x, int y);
#endif
#endif  
	 



