#include "main.h"
#include "wifi.h"
#include <stdio.h>
#include "IEMCS.h"
#include "dma.h"
#include "adc.h"


sensor_data_Typedef sensor_data;
set_data_Typedef set_data;
limit_data_Typedef limit_data;
auto_set_flag_Typedef auto_set_flag;
uint8_t alarm_falg = 0;

uint8_t rDataBuffer[1] ={0};  //  RX Data buffer


/*******************全局静态变量*******************/

static StackType_t OLED_scan_Task_Stack[128];/* OLED_scan任务堆栈 */
static StaticTask_t OLED_scan_Task_TCB;/* OLED_scan任务控制块 */
static TaskHandle_t OLED_scan_Task_Handle;/* OLED_scan任务句柄 */

static StackType_t  App_scan_Task_Stack[512];/* App_scan任务堆栈 */
static StaticTask_t App_scan_Task_TCB;/* App_scan任务控制块 */
static TaskHandle_t App_scan_Task_Handle;/* App_scan任务句柄 */

static StackType_t  IO_scan_Task_Stack[128];/* IO_scan任务堆栈 */
static StaticTask_t IO_scan_Task_TCB;/* IO_scan任务控制块 */
static TaskHandle_t IO_scan_Task_Handle;/* IO_scan任务句柄 */

static StackType_t  data_updata_Task_Stack[128];/* data_updata任务堆栈 */
static StaticTask_t data_updata_Task_TCB;/* data_updata任务控制块 */
static TaskHandle_t data_updata_Task_Handle;

/*******************全局静态函数*******************/
static void AppTaskCreateFun(void); //任务创建函数
static void App_scan_Task (void* parameter); //app的扫描线程
static void OLED_scan_Task (void* parameter);//屏幕的扫描线程
static void IO_scan_Task (void* parameter); //IO的扫描线程
static void data_updata_Task (void* parameter); //IO的扫描线程
static void init_set_data(void);


void SystemClock_Config(void); 
void Delay_ms(uint32_t ms);

int main(void)
{	
	uint8_t status;
	uint8_t rev[1] = {0};
	HAL_Init();
    SystemClock_Config();
	MX_USART1_UART_Init();
	MX_DMA_Init();
	MX_I2C2_Init();
	led_goio_init();
	IO_device_init();
	MX_USART2_UART_Init();	
	wifi_protocol_init();
	HAL_UART_Receive_IT(&huart2, rDataBuffer, 1);
	MX_I2C1_Init();
	SHT30_init();
	MX_RTC_Init();
	OPT3001_init();
	printf(">OPT3001 Init success!\r\n");
	
	MX_ADC1_Init();
	led_on(LED_GREEN);
	Delay_ms(0xffffff);
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	//MQ2_PPM_Calibration();  // 校准MQ传感器
	//MX_TIM3_Init();
	TIM2_Init();
	//HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim2);
	Delay_ms(0xffffff);
	OLED_Init();
	MX_USART2_UART_Init();
	init_set_data();
	wifi_protocol_init();
	HAL_UART_Receive_IT(&huart2, rDataBuffer, 1);
	
	AppTaskCreateFun();
	while(1)
	{
	}
	return 0;
}

static void init_set_data(void)
{
	//eeprom addr	0x20 limit_data.Tempe_max
	//            	ox21 limit_data.Tempe_min;
	//				0x22 limit_data.Humi_max
	//             	0x23 limit_data.Humi_min;
	//				0x24 set_data.light_v H8
	//				0x25 set_data.light_v L8
	//				0x26  set_data.temp_v
	//				0x27  set_data.humi_v
	uint8_t buff[8] = {0};
	I2C_EE_BufferRead(buff,0x20,8);
	limit_data.Tempe_max = buff[0];
	limit_data.Tempe_min = buff[1];
	limit_data.Humi_max = buff[2];
	limit_data.Humi_min = buff[3];
	set_data.light_v = (buff[4]<<8) |buff[5];
	set_data.temp_v = buff[6];
	set_data.humi_v = buff[7];
}

static void AppTaskCreateFun (void)
{
  /* 创建OLED_scan任务 */
	OLED_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t	)OLED_scan_Task,		//任务函数
															(const char* 	)"OLED_scan",		//任务名称
															(uint32_t 		)128,					//任务堆栈大小
															(void* 		  	)NULL,				//传递给任务函数的参数
															(UBaseType_t 	)3, 				//任务优先级
															(StackType_t*   )OLED_scan_Task_Stack,	//任务堆栈
															(StaticTask_t*  )&OLED_scan_Task_TCB);	//任务控制块   
	
	if(NULL != OLED_scan_Task_Handle)/* 创建成功 */
		printf("oled_scan_Task任务创建成功!\n");
  /* 创建APP_scan任务 */
	App_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t)App_scan_Task,		//任务函数
															(const char* 	)"App_scan",		//任务名称
															(uint32_t 		)512,					//任务堆栈大小
															(void* 		  	)NULL,				//传递给任务函数的参数
															(UBaseType_t 	)4, 				//任务优先级
															(StackType_t*   )App_scan_Task_Stack,	//任务堆栈
															(StaticTask_t*  )&App_scan_Task_TCB);	//任务控制块   
	
	if(NULL != App_scan_Task_Handle)/* 创建成功 */
		printf("App_scan_Task任务创建成功!\n");

	  /* 创建IO_scan任务 */
	IO_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t)IO_scan_Task,		//任务函数
															(const char* 	)"IO_scan",		//任务名称
															(uint32_t 		)128,					//任务堆栈大小
															(void* 		  	)NULL,				//传递给任务函数的参数
															(UBaseType_t 	)5, 				//任务优先级
															(StackType_t*   )IO_scan_Task_Stack,	//任务堆栈
															(StaticTask_t*  )&IO_scan_Task_TCB);	//任务控制块   
															
	  /* data_updata_Task */
	data_updata_Task_Handle = xTaskCreateStatic((TaskFunction_t)data_updata_Task,		//任务函数
															(const char* 	)"data_updata",		//任务名称
															(uint32_t 		)128,					//任务堆栈大小
															(void* 		  	)NULL,				//传递给任务函数的参数
															(UBaseType_t 	)1, 				//任务优先级
															(StackType_t*   )data_updata_Task_Stack,	//任务堆栈
															(StaticTask_t*  )&data_updata_Task_TCB);	//任务控制块   
	
	if(NULL != App_scan_Task_Handle)/* 创建成功 */
		printf("data_updata_Task任务创建成功!\n");
	vTaskStartScheduler();   /* 启动任务，开启调度 */
}

uint8_t keyVal = 0xff;
uint8_t hoem_enmuIndex = 0x01; //0x01是home 0x02是菜单
uint8_t enmuIndex = 0x01;//0x01 0x02 0x04 0x08 0x10 0x20
uint8_t ItemIndex = 0x00;
uint8_t opt_cnt = 0;
static void App_scan_Task (void* parameter)
{
	while (1)
    { 		
		wifi_uart_service();
		read_sht30(); //读取温湿度
		opt_cnt  ++; //800ms周期读取光照度
		if(opt_cnt > 80)
		{
			OPT3001_read();
			opt_cnt = 0;
		}
		//
		sensor_data.MQ2 = Get_MQ2_PPM();//读烟雾值
		keyVal = Get_KeyVal();//读键值
		
		/********按键控制**********/
		if(keyVal == 0xa1)
		{
			if(hoem_enmuIndex == 0x01) hoem_enmuIndex =0x02;
			else if(hoem_enmuIndex == 0x02)
			{
				switch(enmuIndex)
				{
					case 0x01:
						ItemIndex = 0x01;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						break;
					case 0x02:
						ItemIndex = 0x02;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						break;
					case 0x04:
						ItemIndex = 0x04;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						keyVal = 0xff;
						break;
					case 0x08:
						ItemIndex = 0x08;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						break;
					case 0x10:
						ItemIndex = 0x10;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						keyVal = 0xff;
						break;
					case 0x20:
						ItemIndex = 0x20;
						enmuIndex = 0x80;
						hoem_enmuIndex = 0;
						break;
					default:ItemIndex = 0x00;break;
				}
			}
			oled_ClearDisplay();
			
		}
		if(keyVal == 0xa2)
		{
			if(hoem_enmuIndex ==0x02)
			{
				enmuIndex = enmuIndex << 1;
				if(enmuIndex == 0x40) enmuIndex = 0x01;
			}
		}
		if(keyVal == 0xa3)
		{
			if(hoem_enmuIndex == 0x02 && enmuIndex != 0x80) 
			{	
				hoem_enmuIndex = 0x01;
				enmuIndex = 0x01;
			}
			if(enmuIndex == 0x80)
			{
				enmuIndex = ItemIndex;
				ItemIndex = 0x00;
				hoem_enmuIndex = 0x02;
			}
			oled_ClearDisplay();
		}
		/*************界面************/
		GUI_MainTask(hoem_enmuIndex); //主界面
		menuWinfun(enmuIndex,hoem_enmuIndex);//菜单界面
		NetInfoWin(ItemIndex);//网络信息界面
		AboutWin(ItemIndex);//关于  界面
		TimeSetWin(ItemIndex,keyVal);//时间校准界面
		DevInfoWin(ItemIndex);//设置信息界面
		DevControl(ItemIndex,keyVal);//设备控制界面
		waringInfo(ItemIndex,keyVal);//警告信息界面
		/*************界面 end************/
		vTaskDelay(10);   /* 延时10个tick */		 		
    }
}


static RTC_TimeTypeDef GetTime;   //获取时间结构体
static RTC_DateTypeDef GetData;  //获取日期结构体
static uint8_t alarm_info[8];
static void IO_scan_Task (void* parameter)
{
	/*****************************
	*****************************/
	while(1)
	{
		/***有关温度的控制**/
		if(auto_set_flag.temp_a)
		{
			if(sensor_data.Temperature <set_data.temp_v - 5)
			{
				IO_on(HERATER_IO);
				set_data.heater_b = 1;
			}
			else if((sensor_data.Temperature >set_data.temp_v + 5))
			{
				IO_off(HERATER_IO);
				set_data.heater_b = 0;
			}
		}
		else
		{
			if(set_data.heater_b) IO_on(HERATER_IO); //如果加热打开那么HERATER_IO开
			else IO_off(HERATER_IO);
			
		}
		/*******************/
		
		/******有关湿度的控制*******/
		if(auto_set_flag.hmui_a)
		{
			if(sensor_data.Humidity <set_data.humi_v- 5)
			{
				IO_on(FAN_IO);
				set_data.fan_b = 1;
			}
			else if(sensor_data.Humidity >set_data.humi_v + 5)
			{
				IO_off(FAN_IO);
				set_data.fan_b = 0;
			}			
		}
		else
		{
			if(set_data.fan_b) IO_on(FAN_IO);
			else IO_off(FAN_IO);
		}
		/*******************************/
		
		/************有关光照的控制***********/
		if(auto_set_flag.light_a)
		{
			if(sensor_data.lux < set_data.light_v)
			{
				IO_on(LIGHT_IO);
				set_data.light_b = 1;
			}
			else
			{
				IO_off(LIGHT_IO);
				set_data.light_b = 0;
			}
				
		}
		else
		{
			if(set_data.light_b) IO_on(LIGHT_IO);
			else IO_off(LIGHT_IO);
		}
		/*************************************/
		
		if(set_data.switch_b) IO_on(SWITCH_IO);
		else IO_off(SWITCH_IO);	

		if(sensor_data.MQ2 > SOMKE_ALARM)
		{
			alarm_falg = 1;
		}
		if(alarm_falg == 1)
		{
			/* Get the RTC current Time */
			HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);
			
			alarm_info[0] = 1;
			alarm_info[1] = 1;
			alarm_info[2] = GetData.Year;
			alarm_info[3] = GetData.Month;
			alarm_info[4] = GetData.Date;
			alarm_info[5] = GetTime.Hours;
			alarm_info[6] = GetTime.Minutes;
			alarm_info[7] = GetTime.Seconds;
			I2C_EE_PageWrite(alarm_info,0x00,8);
			alarm_falg =0;
		}
		
		vTaskDelay(5);
	}
}

static void OLED_scan_Task (void* parameter)
{
	while (1)
    {   
		oled_updataAlldisbuff();
		vTaskDelay(15);   /* 延时500个tick */		 		
    }
}


static void data_updata_Task(void* parameter)
{
	while(1)
	{
		if(mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)
		{
			mcu_dp_value_update(DPID_TEMP_CURRENT,sensor_data.Temperature); //VALUE型数据上报;
			mcu_dp_value_update(DPID_HUMIDITY_VALUE,sensor_data.Humidity); //VALUE型数据上报;
			mcu_dp_value_update(DPID_SMOKE_CONCENTRATION,sensor_data.MQ2); //VALUE型数据上报;
			mcu_dp_value_update(DPID_L,sensor_data.lux); //VALUE型数据上报	
			
			mcu_dp_bool_update(DPID_SWITCH,set_data.switch_b); //BOOL型数据上报;  当前开关 
			mcu_dp_bool_update(DPID_LIGHT,set_data.light_b); //BOOL型数据上报;当前灯
			mcu_dp_bool_update(DPID_FAN,set_data.fan_b); //BOOL型数据上报;当前风扇
			mcu_dp_bool_update(DPID_HEATER,set_data.heater_b); //BOOL型数据上报;当前加热器	
			
			if(sensor_data.Temperature > limit_data.Tempe_max) mcu_dp_enum_update(DPID_TEMP_ALARM,1); //当前温度报警
			else if(sensor_data.Temperature < limit_data.Tempe_min) mcu_dp_enum_update(DPID_TEMP_ALARM,0);
			else mcu_dp_enum_update(DPID_TEMP_ALARM,2);

			if(sensor_data.Humidity >limit_data.Humi_max) mcu_dp_enum_update(DPID_HUM_ALARM,1); //枚举型数据上报;当前湿度报警
			else if(sensor_data.Humidity < limit_data.Humi_min)mcu_dp_enum_update(DPID_HUM_ALARM,0); //枚举型数据上报;当前湿度报警
			else mcu_dp_enum_update(DPID_HUM_ALARM,2); //枚举型数据上报;当前湿度报警

			if(sensor_data.MQ2 > 300)
			{
				mcu_dp_enum_update(DPID_SMLAL,1);
				alarm_falg = 1;
			}
			else
			{
				mcu_dp_enum_update(DPID_SMLAL,2);
			}		
		}
		led_toggle(LED_GREEN);
		//OPT3001_read();
		printf("温度:%.1f,湿度%.1f\r\n",sensor_data.Temperature,sensor_data.Humidity);
		vTaskDelay(3500);		
	}

}
void Delay_ms(uint32_t ms)
{
	for(uint32_t i = ms; i >0; i--)
	{
		
	}
}
/********串口2回调函数**********/	

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance == huart2.Instance)
	{
		uart_receive_input(rDataBuffer[0]);
	}
	while(HAL_UART_Receive_IT(&huart2, rDataBuffer, 1) != HAL_OK); // Wait completly receive 1 byte data, and put data in rDataBuffer
	
	//printf("%c",(rDataBuffer[0]));
	
}
/*tim2回调函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t keycont = 0;
	static uint32_t mtimer1 = 0;
	static uint32_t mtimer2 = 0;
	static uint8_t flag = 1;
	keycont ++;
	mtimer1 ++;
	mtimer2 ++;
	
	if(alarm_falg == 1)
	{
		led_toggle(LED_RED);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
	}
	else
	{
		led_off(LED_RED);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
	}
	if(flag == 1 && mtimer1 > 90000)
	{
		mcu_get_system_time();
		printf("time calireation\r\n");	
		flag = 0;
	}
	if(mtimer2 > 1800000)
	{
		mcu_get_system_time();
		printf("time calireation\r\n");	
		mtimer2 = 0;mtimer1 = 0;
	}
	if(keycont >2)
	{
		KEY_Scan();
		keycont = 0;
	}
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
