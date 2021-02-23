/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
//#define CHANNEL4
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    #ifdef CHANNEL4
    sConfig.Channel = ADC_CHANNEL_4;
    #else
    sConfig.Channel = ADC_CHANNEL_1;

    #endif
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA4     ------> ADC1_IN4
        */
        #ifdef CHANNEL4
		GPIO_InitStruct.Pin = GPIO_PIN_4;
        #else
		GPIO_InitStruct.Pin = GPIO_PIN_1;
        #endif
        
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

    if(adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA4     ------> ADC1_IN4
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
#include <math.h>
#define CAL_PPM 20  //校准环境中PPM值
#define RL 1        //RL阻值

static float R0;    //元件在洁净空气中的阻值


float read_adcVal(void)
{
    uint16_t AD_Value;
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);

    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    {
        AD_Value = HAL_ADC_GetValue(&hadc1);
    }

    return (float)AD_Value / 4096 * (float)3.3;;
}

/***************************************************************
* 函数名称: GetVoltage
* 说    明: 获取PPM函数
* 参    数: 无
*
* 返 回 值: ppm
***************************************************************/

int Get_MQ2_PPM(void)
{
    float voltage, RS, ppm;

    voltage = read_adcVal();
//    RS = (3.3 - voltage) / voltage * RL;    //计算RS
//    ppm = 613.9f * pow(RS / R0, -2.074f); //计算ppm
    return voltage*100;
}

/***************************************************************
* 函数名称: MQ2_PPM_Calibration
* 说    明: 传感器校准函数
* 参    数: 无
*
* 返 回 值: 无
***************************************************************/
void MQ2_PPM_Calibration(void)
{
    float voltage = read_adcVal();
    float RS = (3.3 - voltage) / voltage * RL;
    R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
