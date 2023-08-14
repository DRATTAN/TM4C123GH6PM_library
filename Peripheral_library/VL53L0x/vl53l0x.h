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

//寄存器操作函数
#define VL53L0X_ADDR        0x52 >> 1            //VL53L0x的IIC器件地址
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
    uint16_t acnt;      //环境统计,激光光强
    uint16_t scnt;      //信号数
    uint16_t dist;      //距离，单位mm   最原始数据
    uint8_t DeviceRangeStatusInternal;
    uint16_t dist_last;     //历史有效值，用来判断
    uint16_t distValid;     //原始距离值提取后的有效值
};

extern struct VL53L0xData  VL53L0x_Bottom;      //底部连接在IIC2的VL53L0x的数据



void Vl53l0x_Init(void);

//检查VL53L0x是否正常
uint8_t Vl53l0x_Check(void);

//VL53L0x转换一次
void Vl53l0x_StartConversion(void);

//VL53L0x读取距离等数据反馈信息
void Vl53l0x_ReadDistance(void);

void Vl53l0x_DataFitting(void);


#endif /* PERIPHERAL_LIB_TOF_VL53L0X_H_ */
