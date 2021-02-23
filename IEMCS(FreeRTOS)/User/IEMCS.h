#ifndef __IEMCS_H__
#define __IEMCS_H__

#include "stm32f1xx.h"

#include "usart.h"
#include "i2c.h"
#include "oled.h"
#include "tim.h"
#include "LED.h"
#include "sht30.h"
#include "OPT3001.h"
#include "key.h"
#include "wifi.h"
#include "rtc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "GUI.h"
#include "IO_device.h"
#include "EEPROM.H"

#define LIGHT_IO   IO0
#define HERATER_IO IO1
#define FAN_IO     IO2
#define SWITCH_IO  IO3

#define SOMKE_ALARM 235 //烟雾报警值
typedef struct
{
	float lux;       //光照数据
	float Humidity;  //湿度数据
	float Temperature;//温度数据
	int MQ2;		//烟雾传感器数据
}sensor_data_Typedef;

typedef struct
{
	float Tempe_max;
	float Tempe_min;
	float Humi_max;
	float Humi_min;
}limit_data_Typedef;

typedef struct
{
	int  light_v;
	char temp_v;
	char humi_v;
	
	uint8_t switch_b;
	uint8_t fan_b; //风扇
	uint8_t heater_b; //加热器
	uint8_t light_b ;
}set_data_Typedef;

typedef struct
{
	uint8_t temp_a;
	uint8_t hmui_a;
	uint8_t light_a;
}auto_set_flag_Typedef;

extern sensor_data_Typedef sensor_data;
extern set_data_Typedef set_data;
extern limit_data_Typedef limit_data;
extern auto_set_flag_Typedef auto_set_flag;

void GUI_MainTask(uint8_t execute_falg);
uint8_t menuWinfun(uint8_t index,uint8_t execute_falg);
void NetInfoWin(uint8_t execute_falg);
void DevControl(uint8_t execute_falg,uint8_t keyval );
void DevInfoWin(uint8_t execute_falg);
void TimeSetWin(uint8_t execute_falg,uint8_t val);
void AboutWin(uint8_t execute_falg);
void waringInfo(uint8_t execute_falg,uint8_t keyval );
#endif


//		for(int i = 0; i < 8; i++)
//		{
//					/*x在2-127偏差为-2，在x的前提下y的值是0—>7,1->0...*/
//			drawLine(2-2,8*0+i,127-2,8*0+i,1);
//		}
//		for(int i = 0; i < 8; i++)
//		{
//					/*x在0-1偏差为+126，在x的前提下y的值是0—>0,1->1...*/
//			drawLine(127,8*7+i,126,8*7+i,1);
//		}


