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


/*******************ȫ�־�̬����*******************/

static StackType_t OLED_scan_Task_Stack[128];/* OLED_scan�����ջ */
static StaticTask_t OLED_scan_Task_TCB;/* OLED_scan������ƿ� */
static TaskHandle_t OLED_scan_Task_Handle;/* OLED_scan������ */

static StackType_t  App_scan_Task_Stack[512];/* App_scan�����ջ */
static StaticTask_t App_scan_Task_TCB;/* App_scan������ƿ� */
static TaskHandle_t App_scan_Task_Handle;/* App_scan������ */

static StackType_t  IO_scan_Task_Stack[128];/* IO_scan�����ջ */
static StaticTask_t IO_scan_Task_TCB;/* IO_scan������ƿ� */
static TaskHandle_t IO_scan_Task_Handle;/* IO_scan������ */

static StackType_t  data_updata_Task_Stack[128];/* data_updata�����ջ */
static StaticTask_t data_updata_Task_TCB;/* data_updata������ƿ� */
static TaskHandle_t data_updata_Task_Handle;

/*******************ȫ�־�̬����*******************/
static void AppTaskCreateFun(void); //���񴴽�����
static void App_scan_Task (void* parameter); //app��ɨ���߳�
static void OLED_scan_Task (void* parameter);//��Ļ��ɨ���߳�
static void IO_scan_Task (void* parameter); //IO��ɨ���߳�
static void data_updata_Task (void* parameter); //IO��ɨ���߳�
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
	//MQ2_PPM_Calibration();  // У׼MQ������
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
  /* ����OLED_scan���� */
	OLED_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t	)OLED_scan_Task,		//������
															(const char* 	)"OLED_scan",		//��������
															(uint32_t 		)128,					//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)3, 				//�������ȼ�
															(StackType_t*   )OLED_scan_Task_Stack,	//�����ջ
															(StaticTask_t*  )&OLED_scan_Task_TCB);	//������ƿ�   
	
	if(NULL != OLED_scan_Task_Handle)/* �����ɹ� */
		printf("oled_scan_Task���񴴽��ɹ�!\n");
  /* ����APP_scan���� */
	App_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t)App_scan_Task,		//������
															(const char* 	)"App_scan",		//��������
															(uint32_t 		)512,					//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)4, 				//�������ȼ�
															(StackType_t*   )App_scan_Task_Stack,	//�����ջ
															(StaticTask_t*  )&App_scan_Task_TCB);	//������ƿ�   
	
	if(NULL != App_scan_Task_Handle)/* �����ɹ� */
		printf("App_scan_Task���񴴽��ɹ�!\n");

	  /* ����IO_scan���� */
	IO_scan_Task_Handle = xTaskCreateStatic((TaskFunction_t)IO_scan_Task,		//������
															(const char* 	)"IO_scan",		//��������
															(uint32_t 		)128,					//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)5, 				//�������ȼ�
															(StackType_t*   )IO_scan_Task_Stack,	//�����ջ
															(StaticTask_t*  )&IO_scan_Task_TCB);	//������ƿ�   
															
	  /* data_updata_Task */
	data_updata_Task_Handle = xTaskCreateStatic((TaskFunction_t)data_updata_Task,		//������
															(const char* 	)"data_updata",		//��������
															(uint32_t 		)128,					//�����ջ��С
															(void* 		  	)NULL,				//���ݸ��������Ĳ���
															(UBaseType_t 	)1, 				//�������ȼ�
															(StackType_t*   )data_updata_Task_Stack,	//�����ջ
															(StaticTask_t*  )&data_updata_Task_TCB);	//������ƿ�   
	
	if(NULL != App_scan_Task_Handle)/* �����ɹ� */
		printf("data_updata_Task���񴴽��ɹ�!\n");
	vTaskStartScheduler();   /* �������񣬿������� */
}

uint8_t keyVal = 0xff;
uint8_t hoem_enmuIndex = 0x01; //0x01��home 0x02�ǲ˵�
uint8_t enmuIndex = 0x01;//0x01 0x02 0x04 0x08 0x10 0x20
uint8_t ItemIndex = 0x00;
uint8_t opt_cnt = 0;
static void App_scan_Task (void* parameter)
{
	while (1)
    { 		
		wifi_uart_service();
		read_sht30(); //��ȡ��ʪ��
		opt_cnt  ++; //800ms���ڶ�ȡ���ն�
		if(opt_cnt > 80)
		{
			OPT3001_read();
			opt_cnt = 0;
		}
		//
		sensor_data.MQ2 = Get_MQ2_PPM();//������ֵ
		keyVal = Get_KeyVal();//����ֵ
		
		/********��������**********/
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
		/*************����************/
		GUI_MainTask(hoem_enmuIndex); //������
		menuWinfun(enmuIndex,hoem_enmuIndex);//�˵�����
		NetInfoWin(ItemIndex);//������Ϣ����
		AboutWin(ItemIndex);//����  ����
		TimeSetWin(ItemIndex,keyVal);//ʱ��У׼����
		DevInfoWin(ItemIndex);//������Ϣ����
		DevControl(ItemIndex,keyVal);//�豸���ƽ���
		waringInfo(ItemIndex,keyVal);//������Ϣ����
		/*************���� end************/
		vTaskDelay(10);   /* ��ʱ10��tick */		 		
    }
}


static RTC_TimeTypeDef GetTime;   //��ȡʱ��ṹ��
static RTC_DateTypeDef GetData;  //��ȡ���ڽṹ��
static uint8_t alarm_info[8];
static void IO_scan_Task (void* parameter)
{
	/*****************************
	*****************************/
	while(1)
	{
		/***�й��¶ȵĿ���**/
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
			if(set_data.heater_b) IO_on(HERATER_IO); //������ȴ���ôHERATER_IO��
			else IO_off(HERATER_IO);
			
		}
		/*******************/
		
		/******�й�ʪ�ȵĿ���*******/
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
		
		/************�йع��յĿ���***********/
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
		vTaskDelay(15);   /* ��ʱ500��tick */		 		
    }
}


static void data_updata_Task(void* parameter)
{
	while(1)
	{
		if(mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)
		{
			mcu_dp_value_update(DPID_TEMP_CURRENT,sensor_data.Temperature); //VALUE�������ϱ�;
			mcu_dp_value_update(DPID_HUMIDITY_VALUE,sensor_data.Humidity); //VALUE�������ϱ�;
			mcu_dp_value_update(DPID_SMOKE_CONCENTRATION,sensor_data.MQ2); //VALUE�������ϱ�;
			mcu_dp_value_update(DPID_L,sensor_data.lux); //VALUE�������ϱ�	
			
			mcu_dp_bool_update(DPID_SWITCH,set_data.switch_b); //BOOL�������ϱ�;  ��ǰ���� 
			mcu_dp_bool_update(DPID_LIGHT,set_data.light_b); //BOOL�������ϱ�;��ǰ��
			mcu_dp_bool_update(DPID_FAN,set_data.fan_b); //BOOL�������ϱ�;��ǰ����
			mcu_dp_bool_update(DPID_HEATER,set_data.heater_b); //BOOL�������ϱ�;��ǰ������	
			
			if(sensor_data.Temperature > limit_data.Tempe_max) mcu_dp_enum_update(DPID_TEMP_ALARM,1); //��ǰ�¶ȱ���
			else if(sensor_data.Temperature < limit_data.Tempe_min) mcu_dp_enum_update(DPID_TEMP_ALARM,0);
			else mcu_dp_enum_update(DPID_TEMP_ALARM,2);

			if(sensor_data.Humidity >limit_data.Humi_max) mcu_dp_enum_update(DPID_HUM_ALARM,1); //ö���������ϱ�;��ǰʪ�ȱ���
			else if(sensor_data.Humidity < limit_data.Humi_min)mcu_dp_enum_update(DPID_HUM_ALARM,0); //ö���������ϱ�;��ǰʪ�ȱ���
			else mcu_dp_enum_update(DPID_HUM_ALARM,2); //ö���������ϱ�;��ǰʪ�ȱ���

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
		printf("�¶�:%.1f,ʪ��%.1f\r\n",sensor_data.Temperature,sensor_data.Humidity);
		vTaskDelay(3500);		
	}

}
void Delay_ms(uint32_t ms)
{
	for(uint32_t i = ms; i >0; i--)
	{
		
	}
}
/********����2�ص�����**********/	

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance == huart2.Instance)
	{
		uart_receive_input(rDataBuffer[0]);
	}
	while(HAL_UART_Receive_IT(&huart2, rDataBuffer, 1) != HAL_OK); // Wait completly receive 1 byte data, and put data in rDataBuffer
	
	//printf("%c",(rDataBuffer[0]));
	
}
/*tim2�ص�����*/
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
