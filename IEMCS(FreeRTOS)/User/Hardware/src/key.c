#include "key.h"

#define KEY1     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
#define KEY2     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)
#define KEY3     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)

uint8_t keySta[] = {1,1,1};
uint8_t keyMap[] = {0xa1,0xa2,0xa3};

void key_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*Configure GPIO pins : PB12 PB13 PB14 */
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

void KEY_Scan(void)
{
	uint8_t i;
	static uint8_t key_buff[] ={0xff,0xff,0xff};
	
	key_buff[0]= (key_buff[0] << 1) | KEY1;
	key_buff[1]= (key_buff[1] << 1) | KEY2;
	key_buff[2]= (key_buff[2] << 1) | KEY3;
	for(i = 0; i < 3; i ++)
	{
		if((key_buff[i] & 0xff) == 0xff)
		{
			keySta[i] = 1;
		}
		
		else if((key_buff[i] & 0xff) == 0x00)
		{
			keySta[i] = 0;
		}
		else{}
	}
}

uint8_t Get_KeyVal(void)
{
	uint8_t i, res = 0xff;
	static uint8_t backup[] = {1,1,1};
	
	for(i = 0; i < 3; i ++)
	{
		if(backup[i] != keySta[i])
		{
			if(backup[i] != 0)
			{
				res = keyMap[i];
			}
			else
			{
				res = 0xff;
			}
		}
		backup[i] = keySta[i];
	}
	return res;
}

