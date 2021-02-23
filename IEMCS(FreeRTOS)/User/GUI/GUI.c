#include "GUI.h"



static void check(uint8_t *x, uint8_t *y)
{
	if(*x > MY_DISPLAY_X_WIDTH) *x = MY_DISPLAY_X_WIDTH;
	if(*y > MY_DISPLAY_Y_WIDTH) *y = MY_DISPLAY_Y_WIDTH;
}
void MyGui_drawPos(uint8_t x, uint8_t y, uint8_t color)
{
	
	oled_set_pos((int)x, (int)y, color);
}

/*这里使用了Bresenham算法*/
void MyGUI_DrawLine(uint8_t begin_x,uint8_t begin_y,uint8_t end_x,uint8_t end_y,uint8_t color)
{
	check(&begin_x,&begin_y);
	check(&end_x,&end_y);
	int16_t dx,dy,interchange = 0;//dx是横坐标差，dy是纵坐标差，interchange交换标志
	int16_t deltaY,middle,sign_x,sign_y;//
	int16_t tmp;//临时存储变量
	int16_t x = begin_x;
	int16_t y = begin_y;
	
	dx = abs(end_x - begin_x); //去横坐标的差
	dy = abs(end_y - begin_y); //去纵坐标的差
	
	if(dx < dy)
	{
		tmp = dx;
		dx = dy;
		dy = tmp;
		interchange = 1;
	}
	
	deltaY = (dy << 1);
	middle = dx;
	sign_x = (end_x > begin_x)?1:-1;
	sign_y = (end_y > begin_y)?1:-1;
	
	MyGui_drawPos(x,y,color);
	for(uint16_t i = 1; i <= dx; i++)
	{
		if(interchange == 0)
			x += sign_x;
		else
			y += sign_y;
		if(deltaY >= middle)
		{
			if(interchange == 0)
				y += sign_y;
			else
				y += sign_y;
			middle += (dx << 1);
		}
		deltaY += (dy << 1);
		
		MyGui_drawPos(x,y,color);
	}
	
	
}


/*******画矩形**********/
void MyGUI_DrawRec(uint8_t begin_x,uint8_t begin_y,uint8_t end_x,uint8_t end_y)
{
	MyGUI_DrawLine(begin_x,begin_y,end_x,begin_y,1);
	MyGUI_DrawLine(end_x,begin_y,end_x,end_y,1);
	MyGUI_DrawLine(end_x,end_y,begin_x,end_y,1);
	MyGUI_DrawLine(begin_x,end_y,begin_x,begin_y,1);
}

/*********输出一个字符*************/
void MyGUI_Char(uint8_t x,uint8_t y, char* ch,uint8_t align)
{
	char tmp[2];
	uint8_t i,j;
	uint8_t ascii_6_12Buffer[12];
	uint8_t gb2312_12_12Buffer[24];
	//sprintf(tmp,"%d",ch);
	strcpy(tmp,ch);
	switch(align)
	{
		case 0:
			if((uint8_t)tmp[0]>=0xa1 && (uint8_t)tmp[1] >= 0xa1)
			{
				GetFontGb2312_12_12(tmp[0],tmp[1],gb2312_12_12Buffer);
				for(i = 0; i < 24; i += 2)
				{
					for(j = 0; j <16; j ++){
						if(j < 8)
							MyGui_drawPos(x + j,y,(gb2312_12_12Buffer[i]>>(7-j)) &0x01);
						else
							MyGui_drawPos(x + j,y,(gb2312_12_12Buffer[i +1]>>(15-j)) &0x01);
					}
					y = y + 1;
				}

			}
			else 
			{
				GetFontASCII_6_12(tmp[0],ascii_6_12Buffer);
				for(i = 0; i < 12; i++)
				{
					for(j = 0; j <8; j ++)
						MyGui_drawPos(x + j,y,(ascii_6_12Buffer[i]>>(7-j)) &0x01);
					y = y + 1;
				}
				
			}
		break;
		default:
			if((uint8_t)tmp[0]>=0xa1 && (uint8_t)tmp[1] >= 0xa1)
			{
				GetFontGb2312_12_12(tmp[0],tmp[1],gb2312_12_12Buffer);
				for(i = 0; i < 24; i += 2)
				{
					for(j = 0; j <16; j ++){
						if(j < 8)
							MyGui_drawPos(x + j,y,(~(gb2312_12_12Buffer[i])>>(7-j) &0x01));
						else
							MyGui_drawPos(x + j,y,(~(gb2312_12_12Buffer[i +1])>>(15-j) &0x01));
					}
					y = y + 1;
				}

			}
			else 
			{
				GetFontASCII_6_12(tmp[0],ascii_6_12Buffer);
				for(i = 0; i < 12; i++)
				{
					for(j = 0; j <8; j ++)
						MyGui_drawPos(x + j,y,(~(ascii_6_12Buffer[i])>>(7-j)) &0x01);
					y = y + 1;
				}
				
			}
		break;
	}	
}

/*输出一个字符串*/
void MyGUI_String(uint8_t x,uint8_t y, uint8_t align,char * str)
{
	char tmp[3];
	while(*str != '\0')
	{
		if((uint8_t)*str >= 0xa1)
		{
			tmp[0] = *str;
			tmp[1] = *(str +1);
			tmp[2] = '\0';
			MyGUI_Char(x,y, tmp,align);
			x = x + 12;
			str = str +2;
		}
		else
		{
			tmp[0] = *str;
			tmp[1] = 0;
			tmp [2] = '\0';
			MyGUI_Char(x,y, tmp,align);
			x = x + 6;
			str ++;
		}
		
	}
}


//void MyGUI_Text(uint8_t x,uint8_t y,uint8_t width,text_style_Typedef style,char * str)
//{
//	switch(style.align)
//	{
//		case FONT_LEFT:
//			x= x;
//			break;
//		case FONT_MID:
//			x = width/2 - strlen(str)/2;
//			break;
//		case FONT_RIGHT:
//			x = width - strlen(str);
//			break;
//		default:break;
//	}
//	MyGUI_String(x,y,str);
//}