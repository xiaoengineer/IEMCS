#include "LED.h"

void led_goio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	
	/*LED引脚时钟使能*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	led_off(LED_GREEN);
	led_off(LED_RED);
}

/*开启led*/
void led_on(uint16_t gpio_pin_x)
{
	HAL_GPIO_WritePin(GPIOA,gpio_pin_x,GPIO_PIN_RESET);
}

/*关闭led*/
void led_off(uint16_t gpio_pin_x)
{
	HAL_GPIO_WritePin(GPIOA,gpio_pin_x,GPIO_PIN_SET);
}

/*led 反转*/
void led_toggle(uint16_t gpio_pin_x)
{
	HAL_GPIO_TogglePin(GPIOA, gpio_pin_x);
}