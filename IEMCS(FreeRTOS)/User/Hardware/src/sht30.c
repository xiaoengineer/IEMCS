#include "sht30.h"


#define SHT30_Addr 0x44

/***************************************************************
* 函数名称: SHT3x_CheckCrc
* 说    明: 检查数据正确性
* 参    数: data：读取到的数据
						nbrOfBytes：需要校验的数量
						checksum：读取到的校对比验值
* 返 回 值: 校验结果，0-成功		1-失败
***************************************************************/
static uint8_t SHT3x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
	
    uint8_t crc = 0xFF;
    uint8_t bit = 0;
    uint8_t byteCtr ;
	const int16_t POLYNOMIAL = 0x131;
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
	
}

/***************************************************************
* 函数名称: SHT3x_CalcTemperatureC
* 说    明: 温度计算
* 参    数: u16sT：读取到的温度原始数据
* 返 回 值: 计算后的温度数据
***************************************************************/
static float SHT3x_CalcTemperatureC(uint16_t u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [℃] --
    temperatureC = (175 * (float)u16sT / 65535 - 45); //T = -45 + 175 * rawValue / (2^16-1)
	
    return temperatureC;
	
}

/***************************************************************
* 函数名称: SHT3x_CalcRH
* 说    明: 湿度计算
* 参    数: u16sRH：读取到的湿度原始数据
* 返 回 值: 计算后的湿度数据
***************************************************************/
static float SHT3x_CalcRH(uint16_t u16sRH)
{
	
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    humidityRH = (100 * (float)u16sRH / 65535);  // RH = rawValue / (2^16-1) * 10
	
    return humidityRH;
	
}


/***************************************************************
* 函数名称: SHT30_init
* 说    明: SHT30初始化
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void SHT30_init(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t buff[2] = {0x21,0x30};
	status = HAL_I2C_Master_Transmit(&hi2c1,SHT30_Addr<<1,buff,2,100);
	//status = HAL_I2C_Mem_Write(hi2c1,SHT30_Addr<<1,0x2238,I2C_MEMADD_SIZE_16BIT,);
	if(status != HAL_OK)
	{
		printf("SHT30 init fail!\r\n");
	}
}


void read_sht30(void)
{
    uint8_t data[3];
    uint16_t dat, tmp;
    uint8_t SHT30_data_buffer[6];
	HAL_StatusTypeDef status = HAL_OK;

	HAL_I2C_Mem_Read(&hi2c1,SHT30_Addr<<1,0xe000,I2C_MEMADD_SIZE_16BIT,SHT30_data_buffer,6,100);
	if(status != HAL_OK)
	{
		printf("SHT30 read fail!\r\n");
	}
	//温度数据校验
    data[0] = SHT30_data_buffer[0];
    data[1] = SHT30_data_buffer[1];
    data[2] = SHT30_data_buffer[2];
	
	tmp = SHT3x_CheckCrc(data,2,data[2]);
    if(!tmp)
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        sensor_data.Temperature = SHT3x_CalcTemperatureC(dat);
    }
	
    //湿度数据校验
    data[0] = SHT30_data_buffer[3];
    data[1] = SHT30_data_buffer[4];
    data[2] = SHT30_data_buffer[5]; 
	
    tmp = SHT3x_CheckCrc(data,2,data[2]);
    if(!tmp)
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        sensor_data.Humidity = SHT3x_CalcRH(dat);
    } 
	
}


