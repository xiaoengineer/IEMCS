#include <stdlib.h>
#include <stdio.h>
#include "IEMCS.h"



static RTC_TimeTypeDef GetTime;   //获取时间结构体
static RTC_DateTypeDef GetData;  //获取日期结构体

uint8_t Rcvwifi_test;
//uint8_t rtcRcvFlag = 0;




/***************静态函数************/
static void claer_array(char * array, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		array[i] = 0;
	}
}



void GUI_MainTask(uint8_t execute_falg)
{	
	char textbuff[20];
	if(execute_falg==0x01)
	{
		/* Get the RTC current Time */
		HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);		
		switch(mcu_get_wifi_work_state())
        {
            case SMART_CONFIG_STATE:
                MyGUI_String(2, 0,NO_ANTI,"Wifi:Smart Mode  ");
            break;
            case AP_STATE:
                MyGUI_String(2, 0,NO_ANTI,"Wifi:AP Mode     ");
            break;
            case WIFI_NOT_CONNECTED:
                MyGUI_String(2, 0,NO_ANTI,"Router Connecting");
            break;
            case WIFI_CONNECTED:
                MyGUI_String(2, 0,NO_ANTI,"Router Connected ");
            break;
			case WIFI_CONN_CLOUD:
				MyGUI_String(2, 0,NO_ANTI,"CONN Cloud       ");
			break;
			case WIFI_LOW_POWER:
				MyGUI_String(2, 0,NO_ANTI,"Low Power        ");
			break;
			case SMART_AND_AP_STATE:
				MyGUI_String(2, 0,NO_ANTI,"Smart and AP state  ");
			break;
            default:
				MyGUI_String(2, 0,NO_ANTI,"Unknow           ");
			break;
        }		
		
		claer_array(textbuff,20);
		sprintf(textbuff,"温 度:%.0f℃   ",sensor_data.Temperature);
		MyGUI_String(3, 13,NO_ANTI,textbuff);
		claer_array(textbuff,20);
		sprintf(textbuff,"湿 度:%.0f%%  ",sensor_data.Humidity);
		MyGUI_String(70, 13,NO_ANTI,textbuff);		
		claer_array(textbuff,20);
		if(sensor_data.lux < 1000)
			sprintf(textbuff,"光 强:%.0fLx  ",sensor_data.lux);
		else
			sprintf(textbuff,"光 强:%.1fkL  ",sensor_data.lux/1000);
		MyGUI_String(3, 27,NO_ANTI,textbuff);

		claer_array(textbuff,20);
		if(sensor_data.MQ2 < SOMKE_ALARM-50)
			sprintf(textbuff,"烟 雾:%d   ",0);
		else
			sprintf(textbuff,"烟 雾:%d   ",sensor_data.MQ2);	
		MyGUI_String(70,27,NO_ANTI,textbuff);
		claer_array(textbuff,20);
		
		sprintf(textbuff,"菜 单|%02d:%02d:%02d|%d/%d  ",GetTime.Hours, GetTime.Minutes, GetTime.Seconds, GetData.Month, GetData.Date);
		MyGUI_DrawLine(3, 50,125,50,1);
		MyGUI_DrawLine(3, 12,125,12,1);
		MyGUI_String(3, 51,NO_ANTI,textbuff);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);	
	}

	
}



uint8_t menuWinfun(uint8_t index,uint8_t execute_falg)
{
	if(execute_falg==0x02)
	{
		if(index <= 0x04)
		{
			MyGUI_String(2, 0,NO_ANTI,    " *****MENU(1/2)*****");		
			MyGUI_String(4,13,0x01&index,"1. Network Info    ");	
			MyGUI_String(4,25,0x02&index,"2. Alarm Info      ");	
			MyGUI_String(4,37,0x04&index,"3. Device control  ");		
		}

		if(index >0x04)
		{
			MyGUI_String(0, 0,NO_ANTI,    " *****MENU(2/2)*****");		
			MyGUI_String(4,13,0x08&index,"4. Device Info     ");	
			MyGUI_String(4,25,0x10&index,"5. Time Calibration");	
			MyGUI_String(4,37,0x20&index,"6. About           ");		
		}
		//MyGUI_String(2,56,"时间校准");
		MyGUI_String(3, 51,NO_ANTI,"OK|≡≡[↓]≡≡|Quit");	
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 12,125,12,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);
	
	}
	return index;
}

uint8_t MAC_ADDR[7];
void NetInfoWin(uint8_t execute_falg)
{
	char txetbuff[30] = {0};
	if(execute_falg == 0x01)
	{
		switch(mcu_get_wifi_work_state())
        {
            case SMART_CONFIG_STATE:
                MyGUI_String(2, 5,NO_ANTI,"Wifi:Smart Mode  ");
            break;
            case AP_STATE:
                MyGUI_String(2, 5,NO_ANTI,"Wifi:AP Mode     ");
            break;
            case WIFI_NOT_CONNECTED:
                MyGUI_String(2, 5,NO_ANTI,"Router Connecting");
            break;
            case WIFI_CONNECTED:
                MyGUI_String(2, 5,NO_ANTI,"Router Connected ");
            break;
			case WIFI_CONN_CLOUD:
				MyGUI_String(2, 5,NO_ANTI,"CONN Cloud       ");
			break;
			case WIFI_LOW_POWER:
				MyGUI_String(2, 5,NO_ANTI,"Low Power        ");
			break;
			case SMART_AND_AP_STATE:
				MyGUI_String(2, 5,NO_ANTI,"Smart and AP state  ");
			break;
            default:
				MyGUI_String(2, 5,NO_ANTI,"Unknow           ");
			break;
        }
		
		mcu_get_module_mac();
		if(MAC_ADDR[0] == 0)
		{
			sprintf(txetbuff,"MAC:%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",MAC_ADDR[1],MAC_ADDR[2],MAC_ADDR[3],\
																MAC_ADDR[4],MAC_ADDR[5],MAC_ADDR[6]);
			MyGUI_String(2, 18,NO_ANTI,txetbuff);
		}
		MyGUI_String(3, 51,NO_ANTI,"≡≡≡≡≡≡≡|Quit");
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);	
	}
}

void waringInfo(uint8_t execute_falg,uint8_t keyval )
{
	char textbuff[20];
	uint8_t revInfo[8];
	if(execute_falg == 0x02)
	{
		I2C_EE_BufferRead(revInfo,0x00,8);
		if(revInfo[0] == 1)
		{
			sprintf(textbuff," %02d/%02d/%02d %02d:%02d:%02d",revInfo[2],revInfo[3],revInfo[4],revInfo[5],revInfo[6],revInfo[7]);
		}
		else
		{
			sprintf(textbuff," --/--/-- --:--:--");
		}
		if(keyval == 0xa2)
		{
			I2C_EE_ByteWrite(0,0x00);
		}
		MyGUI_String(6, 0,NO_ANTI,"Last alarm Info");
		MyGUI_String(6, 13,NO_ANTI,"Alarm value: 300ppm");		
		MyGUI_String(3, 25,NO_ANTI,"- - - - - - - - - - ");
		MyGUI_String(3, 31,NO_ANTI,textbuff);
		MyGUI_String(3, 54,NO_ANTI,"≡≡≡≡Del≡≡|Quit");		
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);			
	}
}


void DevControl(uint8_t execute_falg,uint8_t keyval )
{
	static uint8_t pageIndex = 0x01;
	if(execute_falg == 0x04)
	{
		if(keyval == 0xa1)
		{
			switch(pageIndex)
			{
				case 0x01:
					set_data.light_b = ~set_data.light_b;
					mcu_dp_bool_update(DPID_LIGHT,set_data.light_b); //BOOL型数据上报;当前灯
					break;
				case 0x02:
					auto_set_flag.light_a = ~auto_set_flag.light_a;
					mcu_dp_bool_update(DPID_LIGHT_A,auto_set_flag.light_a);
					break;
				case 0x04:
					set_data.heater_b = ~set_data.heater_b;
					mcu_dp_bool_update(DPID_HEATER,set_data.heater_b); //BOOL型数据上报;当前加热器					
					break;
				case 0x08:
					auto_set_flag.temp_a = ~auto_set_flag.temp_a;
					mcu_dp_bool_update(DPID_TEMP_A,auto_set_flag.temp_a); 
					break;
				case 0x10:
					set_data.fan_b = ~set_data.fan_b;
					mcu_dp_bool_update(DPID_FAN,set_data.fan_b); //BOOL型数据上报;当前风扇
					break;
				case 0x20:
					auto_set_flag.hmui_a = ~auto_set_flag.hmui_a;
					mcu_dp_bool_update(DPID_LIGHT_A,auto_set_flag.light_a);
					break;
				case 0x40:
					set_data.switch_b = ~set_data.switch_b;
					mcu_dp_bool_update(DPID_SWITCH,set_data.switch_b);
					break;
			}
		}
		
		if(keyval == 0xa2)
		{
			pageIndex = pageIndex <<1;
			if(0x80 == pageIndex) pageIndex = 0x01;
		}
		if(keyval == 0xa3)
			pageIndex = 0x01;
		
		
		if(set_data.light_b)
			MyGUI_String(10, 0,0x01&pageIndex,"IO0:ON ");
		else
			MyGUI_String(10, 0,0x01&pageIndex,"IO0:OFF");
		
		if(set_data.heater_b)
			MyGUI_String(10,13,0x04&pageIndex,"IO1:0N ");
		else
			MyGUI_String(10,13,0x04&pageIndex,"IO1:OFF");
		
		if(set_data.fan_b)
			MyGUI_String(10,25,0x10&pageIndex,"IO2:ON ");
		else
			MyGUI_String(10,25,0x10&pageIndex,"IO2:OFF");
		
		if(set_data.switch_b)
			MyGUI_String(10,37,0x40&pageIndex,"IO3:ON ");	
		else
			MyGUI_String(10,37,0x40&pageIndex,"IO3:OFF");

		if(auto_set_flag.light_a)
			MyGUI_String(63, 0,0x02&pageIndex,"MODE:AUTO");
		else
			MyGUI_String(63, 0,0x02&pageIndex,"MODE:OPER");
		
		if(auto_set_flag.temp_a)
			MyGUI_String(63,13,0x08&pageIndex,"MODE:AUTO");	
		else
			MyGUI_String(63,13,0x08&pageIndex,"MODE:OPER");	
		
		if(auto_set_flag.hmui_a)
			MyGUI_String(63,25,0x20&pageIndex,"MODE:AUTO");
		else
			MyGUI_String(63,25,0x20&pageIndex,"MODE:OPER");
		
		MyGUI_String(3, 51,NO_ANTI,"OK|≡≡[↓]≡≡|Quit");	
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);		
	}	
}



void DevInfoWin(uint8_t execute_falg)
{
	char textbuff[20];
	if(execute_falg == 0x08)
	{
		claer_array(textbuff,20);

		claer_array(textbuff,20);
		sprintf(textbuff,"T set:%d  ",set_data.temp_v);		
		MyGUI_String(6, 4,NO_ANTI,textbuff);
		
		claer_array(textbuff,20);
		sprintf(textbuff,"H set:%d  ",set_data.humi_v);				
		MyGUI_String(6, 16,NO_ANTI,textbuff);
		
		claer_array(textbuff,20);
		sprintf(textbuff,"L set:%d  ",set_data.light_v);			
		MyGUI_String(6, 29,NO_ANTI,textbuff);

		MyGUI_String(3, 51,NO_ANTI,"≡≡≡≡≡≡≡|Quit");
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);		
	}

}

void TimeSetWin(uint8_t execute_falg,uint8_t val)
{
	char textbuff[20];		
	if(execute_falg == 0x10)
	{
		HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);		
		MyGUI_String(2, 4,NO_ANTI,"   Press \"OK\" to");
		MyGUI_String(2, 17,NO_ANTI,"calibrate the time.");
		claer_array(textbuff,20);
		sprintf(textbuff,"   %02d:%02d:%02d|%d/%d",GetTime.Hours, GetTime.Minutes, GetTime.Seconds, GetData.Month, GetData.Date);
		MyGUI_String(2, 32,NO_ANTI,textbuff);
		if(val == 0xa1)
		{
			mcu_get_system_time();
		}
		MyGUI_String(3, 51,NO_ANTI,"OK|≡≡≡≡≡≡|Quit");	
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);
	}
}

void AboutWin(uint8_t execute_falg)
{
	if(execute_falg == 0x20)
	{
		MyGUI_String(5, 5,NO_ANTI,"Version:V0.0.1");
		MyGUI_String(5, 18,NO_ANTI,"Author :Wangh");
		MyGUI_String(5, 31,NO_ANTI,"Company:XXXX");
		MyGUI_String(3, 51,NO_ANTI,"≡≡≡≡≡≡≡|Quit");
		MyGUI_DrawLine(2, 50,125,50,1);
		MyGUI_DrawLine(2, 50,2,63,1);
		MyGUI_DrawLine(2, 63,125,63,1);
		MyGUI_DrawLine(125, 50,125,63,1);		

	}
}
//static void testWinFun(void *param)
//{
//	
//	uint8_t keyval=Get_KeyVal();
//	uint8_t wifi_state = mcu_get_wifi_work_state();
//	if(wifi_state == SMART_CONFIG_STATE)
//	{
//		GuiRowText(2, 17,125, FONT_LEFT,"配置状态:Smart");
//	}
//	else if(wifi_state == AP_STATE)
//	{
//		GuiRowText(2, 17,125, FONT_LEFT,"配置状态:AP");
//	}
//	else
//	{
//		GuiRowText(2, 17,125, FONT_LEFT,"配置状态:未知");
//	}
//	
//	if(wifi_state == WIFI_CONNECTED)
//	{
//		GuiRowText(2, 32,125, FONT_LEFT,"WIFI已连接");
//	}
//	else
//	{
//		GuiRowText(2, 32,125, FONT_LEFT,"WIFI未连接");
//	}

//	if(keyval == 0xa3)
//	{
//		GuiWinDeleteTop();
//		GuiMenuRedrawMenu(&HmiMenu);
//		printf(">recv mb :%x\r\n",keyval);
//	}	
//}

	
//static void time_calibrationWinFun(void *param)
//{
//	uint8_t keyval=Get_KeyVal();
//	GuiRowText(2, 17,125, FONT_LEFT,"Please pass [OK] key to");
//	GuiRowText(2, 32,125, FONT_LEFT,"calibrate the local time");
//	GuiRowText(2, 51,125, FONT_MID,(uint8_t *)"[OK]≡≡≡≡≡[Quit]");
//	if(keyval ==0xa1 && rtcRcvFlag == 0)
//	{
//		GuiRowText(2, 17,125, FONT_LEFT,"    calibrating...     ");
//		GuiRowText(2, 32,125, FONT_LEFT,"                       ");
//		mcu_get_system_time();
//	}

//	if(keyval == 0xa3)
//	{
//		GuiWinDeleteTop();
//		GuiMenuRedrawMenu(&HmiMenu);
//		printf(">recv mb :%x\r\n",keyval);
//	}
//}

