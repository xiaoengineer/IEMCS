#include "IO_device.h"

void IO_device_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	
	/*IO_device引脚时钟使能*/
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*开启led*/
void IO_on(uint16_t IO_x)
{
	HAL_GPIO_WritePin(GPIOA,IO_x,GPIO_PIN_SET);
	
}

/*关闭led*/
void IO_off(uint16_t IO_x)
{
	HAL_GPIO_WritePin(GPIOA,IO_x,GPIO_PIN_RESET);
}
