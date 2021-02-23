#ifndef __EEPROM_H__
#define __EEPROM_H__


#include "IEMCS.h"

#define I2C_Handle hi2c1

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define I2Cx_TIMEOUT_MAX                300
/* Maximum number of trials for HAL_I2C_IsDeviceReady() function */
#define EEPROM_MAX_TRIALS               300


/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

#define EEPROM_ADDRESS        0xA0

uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr);
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
#endif


