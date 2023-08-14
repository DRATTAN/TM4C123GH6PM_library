/*
 * tof.h
 *
 *  Created on: 2023/07/08
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_TOF_VL53L0X_H_
#define PERIPHERAL_LIB_TOF_VL53L0X_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib_i2c.h"
#include "time.h"

//�Ĵ�����������
#define VL53L0X_ADDR        0x52 >> 1            //VL53L0x��IIC������ַ
#define VL53L0X_I2C         I2C0

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14


struct VL53L0xData {
    uint8_t vtemp[12];
    uint16_t acnt;      //����ͳ��,�����ǿ
    uint16_t scnt;      //�ź���
    uint16_t dist;      //���룬��λmm   ��ԭʼ����
    uint8_t DeviceRangeStatusInternal;
    uint16_t dist_last;     //��ʷ��Чֵ�������ж�
    uint16_t distValid;     //ԭʼ����ֵ��ȡ�����Чֵ
};

extern struct VL53L0xData  VL53L0x_Bottom;      //�ײ�������IIC2��VL53L0x������



void Vl53l0x_Init(void);

//���VL53L0x�Ƿ�����
uint8_t Vl53l0x_Check(void);

//VL53L0xת��һ��
void Vl53l0x_StartConversion(void);

//VL53L0x��ȡ��������ݷ�����Ϣ
void Vl53l0x_ReadDistance(void);

void Vl53l0x_DataFitting(void);


#endif /* PERIPHERAL_LIB_TOF_VL53L0X_H_ */
