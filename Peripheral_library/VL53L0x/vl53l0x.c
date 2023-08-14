/*
 *  TOF.c
 *
 *  Created on: 2023/07/08
 *      Author: RATTAN
 */

#include "vl53l0x.h"

struct VL53L0xData  VL53L0x_Bottom;     //�ײ�������IIC2��VL53L0x������


void Vl53l0x_Init()
{
    uint8_t VL53L0x_SendData[2] = {0x01};
    uint8_t VL53L0x_RecData[5];

    LIB_I2C_Transmit(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_SYSRANGE_START, VL53L0x_SendData, 1);

    VL53L0x_SendData[1] = 100;
    while(VL53L0x_SendData[1]--)
    {
        delay_ms(1);
        LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_RESULT_RANGE_STATUS, VL53L0x_RecData+4, 1);
        if(VL53L0x_RecData[4] & 0x01) break;
    }
}

//���VL53L0x�Ƿ�����
//���ɹ�����0
//���ʧ�ܷ���1
uint8_t Vl53l0x_Check()
{
    uint8_t VL53L0x_SendData[2] = {0x01};
    uint8_t VL53L0x_RecData[5] ;

    //Revision ID:
    LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_IDENTIFICATION_REVISION_ID, VL53L0x_RecData, 1);
    //Device ID:
    LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_IDENTIFICATION_MODEL_ID, VL53L0x_RecData+1, 1);
    //PRE_RANGE_CONFIG_VCSEL_PERIOD =
    LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD, VL53L0x_RecData+2, 1);
    //FINAL_RANGE_CONFIG_VCSEL_PERIOD=
    LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD, VL53L0x_RecData+3, 1);

    LIB_I2C_Transmit(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_SYSRANGE_START, VL53L0x_SendData, 1);
    delay_ms(500);

    VL53L0x_SendData[1] = 100;
    while(VL53L0x_SendData[1]--)
    {
        delay_ms(10);
        LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_RESULT_RANGE_STATUS, VL53L0x_RecData+4, 1);
        if(VL53L0x_RecData[4] & 0x01) break;
    }

    if (VL53L0x_RecData[4] & 0x01)
    return 0;               //���ɹ�����0
    else return 1;  //���ʧ�ܷ���1
}

//VL53L0xת��һ��
void Vl53l0x_StartConversion()
{
    uint8_t VL53L0x_SendData[1] = {0x01};
    LIB_I2C_Transmit(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_SYSRANGE_START, VL53L0x_SendData, 1);
}

uint16_t makeuint16(int lsb, int msb)
{
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}


//VL53L0x��ȡ��������ݷ�����Ϣ
void Vl53l0x_ReadDistance()
{
    //��¼��ʷ��Чֵ
    VL53L0x_Bottom.dist_last = VL53L0x_Bottom.distValid;

    LIB_I2C_Recive(VL53L0X_I2C, VL53L0X_ADDR, VL53L0X_REG_RESULT_RANGE_STATUS, VL53L0x_Bottom.vtemp, 12);

    VL53L0x_Bottom.acnt = makeuint16(VL53L0x_Bottom.vtemp[7], VL53L0x_Bottom.vtemp[6]);
    VL53L0x_Bottom.scnt = makeuint16(VL53L0x_Bottom.vtemp[9], VL53L0x_Bottom.vtemp[8]);
    VL53L0x_Bottom.dist = makeuint16(VL53L0x_Bottom.vtemp[11], VL53L0x_Bottom.vtemp[10]);
    VL53L0x_Bottom.DeviceRangeStatusInternal = ((VL53L0x_Bottom.vtemp[0] & 0x78) >> 3);

    //��ȡ��Чֵ
    if(VL53L0x_Bottom.dist <= 0x0014)     //����������Ч
        VL53L0x_Bottom.distValid = VL53L0x_Bottom.dist_last;
    else    //��Ч
        VL53L0x_Bottom.distValid = VL53L0x_Bottom.dist;
}
