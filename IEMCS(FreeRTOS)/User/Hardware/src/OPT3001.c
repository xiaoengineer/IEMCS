#include "OPT3001.h"

#define OPT3001_ADDR 0X8A

void get_OPT3001_Manufacturer_ID(void)
{
	uint8_t data_buffer[2];
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Mem_Read(&hi2c1,OPT3001_ADDR|0X01,0X7E,I2C_MEMADD_SIZE_8BIT,data_buffer,2,1000);
	if(status != HAL_OK)
	{
		printf("OPT3001 Manufacturer ID read fail!\r\n");
	}
	printf("The OPT3001 Manufacturer ID is %c%c\r\n",data_buffer[0],data_buffer[1]);
}


void get_OPT3001_device_ID(void)
{
	uint8_t data_buffer[2];
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Mem_Read(&hi2c1,OPT3001_ADDR|0X01,0X7f,I2C_MEMADD_SIZE_8BIT,data_buffer,2,1000);
	if(status != HAL_OK)
	{
		printf("OPT3001 device ID read fail!\r\n");
	}
	printf("The OPT3001 device ID is %x%x\r\n",data_buffer[0],data_buffer[1]);
}



void OPT3001_init(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t buff[2] = {0xcc,0x10};
	status = HAL_I2C_Mem_Write(&hi2c1,OPT3001_ADDR,0x01,I2C_MEMADD_SIZE_8BIT,buff,2,1000);
	if(status != HAL_OK)
	{
		printf("OPT3001 configuration fail!\r\n");
	}
}

void OPT3001_read(void)
{
	uint8_t data_buffer[2];
	uint16_t exponent;
	uint16_t fractional;
	float lux;
	
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Mem_Read(&hi2c1,OPT3001_ADDR|0X01,0X00,I2C_MEMADD_SIZE_8BIT,data_buffer,2,1000);
	if(status != HAL_OK)
	{
		printf("OPT3001 read fail!\r\n");
	}
	else
	{
		exponent   = (data_buffer[0] &0xf0)>>4;
		fractional = ((data_buffer[0] &0x0f) << 8)| data_buffer[1];
		lux = 0.01 * (0x01 << exponent) *fractional;
		sensor_data.lux = lux *1.1;
	}
}




