#include "oled.h"
#include "stdlib.h"
//#include "oledfont.h"
#include "i2c.h"
#include "stdio.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/*显示缓存区*/
uint8_t displayBuff[Y_WIDTH/8][X_WIDTH] ={0};
uint8_t I2C2_MemTxFinshFlag = 1;
#ifdef USE_STM32_IICDMA

uint8_t oled_cmd[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xDB, 0x30, 0x8D, 0x14, 0xAF};

void  HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	I2C2_MemTxFinshFlag = 1;
}

static void OLED_WR_Byte(unsigned char dat, unsigned char cmd)
{
    HAL_StatusTypeDef status = HAL_OK;
	uint8_t pbuff[1] = {dat};
	if(cmd)
    {
        //Write_IIC_Data(dat);
		status = HAL_I2C_Mem_Write(&hi2c2,0x78,0x40,I2C_MEMADD_SIZE_8BIT,pbuff,1,100);
    }
    else
    {
        //Write_IIC_Command(dat);
		status = HAL_I2C_Mem_Write(&hi2c2,0x78,0x00,I2C_MEMADD_SIZE_8BIT,pbuff,1,100);
    }


}


static void oled_not_dmawriteCmd(void)
{

	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	
    OLED_WR_Byte(0x00, OLED_CMD); 
    OLED_WR_Byte(0x10, OLED_CMD); 

    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    //OLED_WR_Byte(0xF8, OLED_CMD); //在使用DMA之后，屏幕出现了偏移，不清楚是怎么会使，所以只能用这种折中的办法了
	OLED_WR_Byte(0x00, OLED_CMD);

    OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel	
 	OLED_WR_Byte(0x20, OLED_CMD);
	OLED_WR_Byte(0x00, OLED_CMD);	   

}


void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

void oled_updataAlldisbuff(void)
{
	uint32_t count = 0;
	if(HAL_I2C_GetState(&hi2c2) == HAL_I2C_STATE_READY )//&&
	{
		int res = HAL_I2C_Mem_Write_DMA(&hi2c2,0x78,0x40,I2C_MEMADD_SIZE_8BIT,(uint8_t*)displayBuff,sizeof(displayBuff));
		if(res != HAL_OK)
		{
			printf("HAL_I2C_Mem_Write_DMA DATA message is %d\r\n",res);
		}
		I2C2_MemTxFinshFlag =0;		
	}
}
void RangeUpdateDisplay(int beginx, int beginy, int endx, int endy)
{

}
void OLED_Init(void)
{
	oled_not_dmawriteCmd();
	//OLED_Set_Pos(0,0);
}
void oled_ClearDisplay(void)
{
	uint8_t x =0, y= 0;
	
	for(y = 0; y < (Y_WIDTH / 8); y++)
	{
		OLED_Set_Pos(0,y);
		for(x= 0; x < 128; x++)
		{
			displayBuff[y][x] = 0;		
		}
	}
	
}
/*画点**/
void oled_set_pos(int x, int y, int color)
{
	if((x > X_WIDTH)||(y>Y_WIDTH))
	{
		return;
	}
	/**********这一部分是因为在加入DMA后做的偏移*******/
	if(x >= 2)
	{
		x=x-2;
		if(y < 56)
		{
			y = 8 + y;
		}
		else
		{
			y = y - 56;
		}
	}
	else
	{
		x = x + 126;
	}
	/*************************************************/
	if(color)
	{
		displayBuff[y/8][x] |= (0x01 << (y%8));
		//displayBuff[y/8][x] |= (0x80 >> (y%8));
	}
	else
	{
		displayBuff[y/8][x] &= ~(0x01 << (y%8));
	}
}

/**读取一个点的值*/
int oled_get_pos(int x, int y)
{
	if((displayBuff[y/8][x])&(0x01 << (y%8)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#else
void OLED_WR_Byte(unsigned char dat, unsigned char cmd)
{
    HAL_StatusTypeDef status = HAL_OK;
	uint8_t pbuff[1] = {dat};
	if(cmd)
    {
        //Write_IIC_Data(dat);
		status = HAL_I2C_Mem_Write(&hi2c2,0x78,0x40,I2C_MEMADD_SIZE_8BIT,pbuff,1,100);
    }
    else
    {
        //Write_IIC_Command(dat);
		status = HAL_I2C_Mem_Write(&hi2c2,0x78,0x00,I2C_MEMADD_SIZE_8BIT,pbuff,1,100);
    }


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char m, n;

    for(m = 0; m < 8; m++)
    {
        OLED_WR_Byte(0xb0 + m, 0);		//page0-page1
        OLED_WR_Byte(0x00, 0);		//low column start address
        OLED_WR_Byte(0x10, 0);		//high column start address

        for(n = 0; n < 128; n++)
        {
            OLED_WR_Byte(fill_Data, 1);
        }
    }
}


//坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i, n;

    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~7）
        OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
        OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址

        for(n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
    } //更新显示
}
void OLED_On(void)
{
    uint8_t i, n;

    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~7）
        OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
        OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址

        for(n = 0; n < 128; n++)OLED_WR_Byte(1, OLED_DATA);
    } //更新显示
}

//初始化SSD1306
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //

    OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}


/***有关移植GUI的函数**/
/*显示缓存区*/
uint8_t displayBuff[Y_WIDTH/8][X_WIDTH];

/*画一个点*/
void oled_set_pos(int x, int y, int color)
{
	if((x > X_WIDTH)||(y>Y_WIDTH))
	{
		return;
	}
	if(color)
	{
		displayBuff[y/8][x] |= (0x01 << (y%8));
	}
	else
	{
		displayBuff[y/8][x] &= ~(0x01 << (y%8));
	}
}

/*读取一个点是否亮*/
int oled_get_pos(int x, int y)
{
	if((displayBuff[y/8][x])&(0x01 << (y%8)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*局部刷新*/
void RangeUpdateDisplay(int beginx, int beginy, int endx, int endy)
{
	unsigned char x, y0,y1;

	if((endx > X_WIDTH)||(endy>Y_WIDTH))
	{
		return;
	}
	if((beginx > endx) || (beginy > endy))
	{
		return;
	}
	
	if(beginy % 8 == 0) y0 = beginy / 8;
	else y0 = beginy / 8 + 1;
	
	if(endy % 8 == 0) y1 = endy / 8;
	else y1 = endy / 8 + 1;
	
	for(uint8_t i =  y0; i < y1; i++)
	{
		OLED_Set_Pos(beginx,i);
		for(x= beginx; x < endx; x++)
		{
			OLED_WR_Byte(displayBuff[i][x], OLED_DATA);			
		}		
	}
	
}

/*全局刷新屏幕*/
void oled_updataAlldisbuff(void)
{
	uint8_t x =0, y= 0;
	
	for(y = 0; y < (Y_WIDTH / 8); y++)
	{
		OLED_Set_Pos(0,y);
		for(x= 0; x < 128; x++)
		{
			OLED_WR_Byte(displayBuff[y][x], OLED_DATA);			
		}
	}
	
}

/*清屏清缓存*/
void oled_ClearDisplay(void)
{
	uint8_t x =0, y= 0;
	
	for(y = 0; y < (Y_WIDTH / 8); y++)
	{
		OLED_Set_Pos(0,y);
		for(x= 0; x < 128; x++)
		{
			displayBuff[y][x] = 0;		
		}
	}
	OLED_Clear();
	
}

#endif

























