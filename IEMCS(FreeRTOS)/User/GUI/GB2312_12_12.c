#include <stdint.h>

#define GB2312_NUM 39
#define GB2312_EACH_SIZE 24
/*
 * gb2312 12_12 ��ģ��.
 * ɨ�跽ʽ�������ң����ϵ��¡����Ȱ��ֽڶ��롣
 * �洢��ʽ: ��λ��ǰ��
 * ȥ����AA-AF��Ԥ������
*/
const unsigned char gb2312_12_12[GB2312_NUM][GB2312_EACH_SIZE] =
{
    /* 0xCEC2 [��]   [3700]*/
    0x0F, 0x80, 0x88, 0x80, 0x4F, 0x80, 0x08, 0x80, 0x0F, 0x80, 0x80, 0x00,
    0x5F, 0xC0, 0x15, 0x40, 0x35, 0x40, 0x55, 0x40, 0x95, 0x40, 0x3F, 0xE0,
    /* 0xCAAA [ʪ]   [3300]*/
    0x80, 0x00, 0x5F, 0xC0, 0x10, 0x40, 0x1F, 0xC0, 0x90, 0x40, 0x5F, 0xC0,
    0x05, 0x00, 0x25, 0x20, 0x15, 0x40, 0x45, 0x00, 0x85, 0x00, 0x3F, 0xE0,
    /* 0xB6C8 [��]   [1450]*/
    0x02, 0x00, 0x7F, 0xE0, 0x48, 0x80, 0x7F, 0xE0, 0x48, 0x80, 0x4F, 0x80,
    0x40, 0x00, 0x5F, 0xC0, 0x48, 0x40, 0x44, 0x80, 0x43, 0x00, 0x9C, 0xE0,
    /* 0xB9E2 [��]   [1758]*/
    0x04, 0x00, 0x44, 0x40, 0x24, 0x80, 0x15, 0x00, 0x04, 0x00, 0xFF, 0xE0,
    0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x21, 0x20, 0x41, 0x20, 0x80, 0xE0,
    /* 0xD5D5 [��]   [4377]*/
    0x00, 0x00, 0xF7, 0xE0, 0x92, 0x20, 0x92, 0x20, 0xF4, 0xC0, 0x98, 0x00,
    0x97, 0xE0, 0x94, 0x20, 0xF7, 0xE0, 0x00, 0x00, 0x52, 0x40, 0x89, 0x20,
    /* 0xD1CC [��]   [3992]*/
    0x2F, 0xE0, 0x28, 0x20, 0x29, 0x20, 0xB9, 0x20, 0xAF, 0xE0, 0xA9, 0x20,
    0xA9, 0x20, 0x2A, 0xA0, 0x2C, 0x60, 0x58, 0x20, 0x4F, 0xE0, 0x88, 0x20,
    /* 0xCEED [��]   [3743]*/
    0x7F, 0xC0, 0x04, 0x00, 0xFF, 0xE0, 0xA4, 0xA0, 0x3F, 0x80, 0x51, 0x00,
    0x0E, 0x00, 0x31, 0x80, 0xC8, 0x60, 0x3F, 0x80, 0x10, 0x80, 0x61, 0x80,
    /* 0xB4AB [��]   [1233]*/
    0x11, 0x00, 0x11, 0x00, 0x2F, 0xC0, 0x22, 0x00, 0x62, 0x00, 0xBF, 0xE0,
    0x24, 0x00, 0x27, 0xC0, 0x20, 0x40, 0x26, 0x80, 0x21, 0x80, 0x20, 0x40,
    /* 0xB8D0 [��]   [1646]*/
    0x01, 0x40, 0x7F, 0xE0, 0x41, 0x00, 0x5D, 0x40, 0x41, 0x40, 0x5C, 0x80,
    0x54, 0xA0, 0x5D, 0x60, 0x82, 0x20, 0x54, 0x40, 0x52, 0xA0, 0x8F, 0xA0,
    /* 0xC6F7 [��]   [3001]*/
    0x7B, 0xC0, 0x4A, 0x40, 0x7B, 0xC0, 0x04, 0x80, 0x04, 0x40, 0xFF, 0xE0,
    0x0A, 0x00, 0x31, 0x80, 0xC0, 0x60, 0x7B, 0xC0, 0x4A, 0x40, 0x7B, 0xC0,
    /* 0xC1AA [��]   [2454]*/
    0x04, 0x40, 0xFA, 0x80, 0x50, 0x00, 0x57, 0xC0, 0x71, 0x00, 0x51, 0x00,
    0x7F, 0xE0, 0x51, 0x00, 0x5A, 0x80, 0xF2, 0x80, 0x14, 0x40, 0x18, 0x20,
    /* 0xC5E4 [��]   [2888]*/
    0x00, 0x00, 0xFE, 0xE0, 0x28, 0x20, 0xFE, 0x20, 0xAA, 0x20, 0xAA, 0xE0,
    0xC6, 0x80, 0x82, 0x80, 0xFE, 0x80, 0x82, 0xA0, 0xFE, 0xA0, 0x82, 0x60,
    /* 0xCDF8 [��]   [3660]*/
    0xFF, 0xE0, 0x80, 0x20, 0x80, 0x20, 0xC4, 0x60, 0xAA, 0xA0, 0x91, 0x20,
    0x91, 0x20, 0xAA, 0xA0, 0xC4, 0x60, 0x80, 0x20, 0x80, 0x20, 0x80, 0xE0,
    /* 0xC7BF [ǿ]   [3039]*/
    0x07, 0xC0, 0xE4, 0x40, 0x24, 0x40, 0x27, 0xC0, 0xE1, 0x00, 0x8F, 0xE0,
    0x89, 0x20, 0xE9, 0x20, 0x2F, 0xE0, 0x21, 0x00, 0x21, 0x40, 0xCF, 0xE0,
    /* 0xA1E6 [��]   [70]*/
    0x40, 0x00, 0xA7, 0x40, 0x48, 0xC0, 0x10, 0x40, 0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x20, 0x08, 0x40, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00,
    /* 0xA1E3 [��]   [67]*/
    0x00, 0x00, 0x30, 0x00, 0x48, 0x00, 0x48, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xB2CB [��]   [1077]*/
    0x11, 0x00, 0xFF, 0xE0, 0x11, 0x00, 0x01, 0xC0, 0xFE, 0x00, 0x48, 0x80,
    0x25, 0x00, 0x04, 0x00, 0xFF, 0xE0, 0x15, 0x00, 0x24, 0x80, 0xC4, 0x60,
    /* 0xB5A5 [��]   [1321]*/
    0x20, 0x80, 0x11, 0x00, 0x7F, 0xC0, 0x44, 0x40, 0x7F, 0xC0, 0x44, 0x40,
    0x7F, 0xC0, 0x04, 0x00, 0xFF, 0xE0, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00,
    /* 0xA1D1 [��]   [49]*/
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x21, 0x00, 0x40, 0x80, 0x4C, 0x80,
    0x4C, 0x80, 0x40, 0x80, 0x21, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 0xA1FD [��]   [93]*/
    0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00,
    0x15, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
    /* 0xA1D4 [��]   [52]*/
    0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 0xC5E4 [��]   [2888]*/
    0x00, 0x00, 0xFE, 0xE0, 0x28, 0x20, 0xFE, 0x20, 0xAA, 0x20, 0xAA, 0xE0,
    0xC6, 0x80, 0x82, 0x80, 0xFE, 0x80, 0x82, 0xA0, 0xFE, 0xA0, 0x82, 0x60,

    /* 0xCDF8 [��]   [3660]*/
    0xFF, 0xE0, 0x80, 0x20, 0x80, 0x20, 0xC4, 0x60, 0xAA, 0xA0, 0x91, 0x20,
    0x91, 0x20, 0xAA, 0xA0, 0xC4, 0x60, 0x80, 0x20, 0x80, 0x20, 0x80, 0xE0,
    /* 0xD5FD [��]   [4417]*/
    0x00, 0x00, 0x7F, 0xC0, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x24, 0x00,
    0x27, 0xC0, 0x24, 0x00, 0x24, 0x00, 0x24, 0x00, 0x24, 0x00, 0xFF, 0xE0,
    /* 0xD2EC [��]   [4118]*/
    0x7F, 0xC0, 0x40, 0x40, 0x7F, 0xC0, 0x40, 0x00, 0x40, 0x20, 0x3F, 0xE0,
    0x10, 0x80, 0x10, 0x80, 0xFF, 0xE0, 0x10, 0x80, 0x20, 0x80, 0x40, 0x80,
    /* 0xB3A3 [��]   [1131]*/
    0x24, 0x80, 0x15, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0x3F, 0x80, 0x20, 0x80,
    0x3F, 0x80, 0x04, 0x00, 0x7F, 0xC0, 0x44, 0x40, 0x44, 0xC0, 0x04, 0x00,
    /* 0xD0C5 [��]   [3891]*/
    0x22, 0x00, 0x21, 0x00, 0x5F, 0xE0, 0x40, 0x00, 0xCF, 0xC0, 0x40, 0x00,
    0x4F, 0xC0, 0x40, 0x00, 0x4F, 0xC0, 0x48, 0x40, 0x4F, 0xC0, 0x48, 0x40,
    /* 0xCFA2 [Ϣ]   [3762]*/
    0x08, 0x00, 0x7F, 0xC0, 0x40, 0x40, 0x7F, 0xC0, 0x40, 0x40, 0x7F, 0xC0,
    0x40, 0x40, 0x7F, 0xC0, 0x04, 0x00, 0x52, 0x40, 0x50, 0xA0, 0x8F, 0xA0,
    /* 0xD6C3 [��]   [4453]*/
    0x7F, 0xC0, 0x4A, 0x40, 0x7F, 0xC0, 0x04, 0x00, 0xFF, 0xE0, 0x04, 0x00,
    0x3F, 0x80, 0x20, 0x80, 0x20, 0x80, 0x24, 0x80, 0x24, 0x80, 0xFF, 0xE0,
    /* 0xD7B4 [״]   [4532]*/
    0x21, 0x00, 0x21, 0x40, 0xA1, 0x20, 0x61, 0x00, 0x2F, 0xE0, 0x21, 0x00,
    0x22, 0x80, 0x62, 0x80, 0xA2, 0x80, 0x24, 0x40, 0x24, 0x40, 0x28, 0x20,
    /* 0xCCAC [̬]   [3490]*/
    0x04, 0x00, 0x04, 0x00, 0xFF, 0xE0, 0x0A, 0x00, 0x11, 0x00, 0x24, 0x80,
    0xC2, 0x60, 0x04, 0x00, 0x52, 0x40, 0x52, 0x20, 0x90, 0xA0, 0x0F, 0x80,
    /* 0xCEB4 [δ]   [3686]*/
    0x04, 0x00, 0x04, 0x00, 0x7F, 0xC0, 0x04, 0x00, 0x04, 0x00, 0xFF, 0xE0,
    0x0E, 0x00, 0x15, 0x00, 0x24, 0x80, 0xC4, 0x60, 0x04, 0x00, 0x04, 0x00,
    /* 0xD6AA [֪]   [4428]*/
    0x20, 0x00, 0x20, 0x00, 0x3D, 0xE0, 0x51, 0x20, 0x91, 0x20, 0x11, 0x20,
    0xFF, 0x20, 0x11, 0x20, 0x29, 0x20, 0x25, 0xE0, 0x45, 0x20, 0x80, 0x00,
    /* 0xC1AC [��]   [2456]*/
    0x82, 0x00, 0x42, 0x00, 0x1F, 0xE0, 0x04, 0x00, 0x09, 0x00, 0xCF, 0xC0,
    0x41, 0x00, 0x41, 0x00, 0x5F, 0xE0, 0x41, 0x00, 0x41, 0x00, 0xBF, 0xE0,
    /* 0xBDD3 [��]   [2119]*/
    0x22, 0x00, 0x21, 0x00, 0x2F, 0xE0, 0xF4, 0x40, 0x22, 0x80, 0x2F, 0xE0,
    0x31, 0x00, 0xEF, 0xE0, 0x22, 0x40, 0x26, 0x40, 0x21, 0x80, 0x6E, 0x60,
    /* 0xCAB1 [ʱ]   [3307]*/
    0x00, 0x40, 0xF0, 0x40, 0x90, 0x40, 0x97, 0xE0, 0x90, 0x40, 0xF0, 0x40,
    0x92, 0x40, 0x91, 0x40, 0x91, 0x40, 0xF0, 0x40, 0x90, 0x40, 0x01, 0xC0,
    /* 0xBCE4 [��]   [2042]*/
    0x4F, 0xE0, 0x20, 0x20, 0x00, 0x20, 0x5F, 0xA0, 0x50, 0xA0, 0x50, 0xA0,
    0x5F, 0xA0, 0x50, 0xA0, 0x50, 0xA0, 0x5F, 0xA0, 0x40, 0x20, 0x40, 0xE0,
    /* 0xD0A3 [У]   [3857]*/
    0x22, 0x00, 0x21, 0x00, 0x2F, 0xE0, 0xF0, 0x00, 0x22, 0x80, 0x74, 0x40,
    0x6A, 0xA0, 0xA2, 0x80, 0x22, 0x80, 0x21, 0x00, 0x22, 0x80, 0x2C, 0x60,
    /* 0xD7BC [׼]   [4540]*/
    0x05, 0x00, 0x04, 0x80, 0x8F, 0xE0, 0x49, 0x00, 0x19, 0x00, 0x2F, 0xC0,
    0x09, 0x00, 0x09, 0x00, 0x2F, 0xC0, 0x49, 0x00, 0x89, 0x00, 0x0F, 0xE0,
};

static uint8_t get_arrayIndex(uint16_t code, uint16_t array[])
{
    uint8_t index = 255;
    uint8_t i = 0;

    do
    {
        if(code == array[i])
        {
            index = i;
            break;
        }
        else
        {
            i++;
        }
    }
    while(i < 255);

    return index;
}

/**
  *@brief ��ȡGB2312 12*12����ģ��
  *@param  codeH ��ģ�����
  *@param  codeH ��ģ�����
  *@param  pBuff ��ģbuff
  *@retval 0
  */
uint8_t GetFontGb2312_12_12(uint8_t codeH, uint8_t codeL, uint8_t *pBuff)
{
    uint16_t offset;
    uint16_t fontCodeCv[GB2312_NUM] = {0xCEC2, 0xCAAA, 0xB6C8, 0xB9E2, 0xD5D5,
                                       0xD1CC, 0xCEED, 0xB4AB, 0xB8D0, 0xC6F7,
                                       0xC1AA, 0xC5E4, 0xCDF8, 0xC7BF, 0xA1E6,
                                       0xA1E3, 0xB2CB, 0xB5A5, 0xA1D1, 0xA1FD, 0xA1D4, 0xC5E4, 0xCDF8, 0xD5FD,
                                       0xD2EC, 0xB3A3, 0xD0C5, 0xCFA2, 0xD6C3, 0xD7B4, 0xCCAC, 0xCEB4, 0xD6AA, 
                                       0xC1AC, 0xBDD3, 0xCAB1, 0xBCE4, 0xD0A3, 0xD7BC
                                      };
    offset = get_arrayIndex(codeH << 8 | codeL, fontCodeCv);

    if(offset > GB2312_NUM)
    {
        for(uint16_t i = 0; i < GB2312_EACH_SIZE; i++)
        {
            pBuff[i] = 1;
        }
    }
    else
    {
        for(uint16_t i = 0; i < GB2312_EACH_SIZE; i++)
        {
            pBuff[i] = gb2312_12_12[offset][i];
        }
    }

    return 0;
}

/* END */
