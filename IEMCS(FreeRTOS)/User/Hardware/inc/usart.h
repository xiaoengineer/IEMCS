#ifndef __USART_H__
#define __USART_H__

#include "stm32f1xx.h"
#include "main.h"
#include <stdio.h>
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);


#endif
