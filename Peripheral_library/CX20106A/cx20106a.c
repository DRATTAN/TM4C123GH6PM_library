/*
 * cx20106a.c
 *
 *  Created on: 2023/07/13
 *      Author: RATTAN
 */


#include "cx20106a.h"

static volatile uint8_t distance_ok = 0;
static uint16_t distance;
static uint8_t distence_buffer[3];
void Cx20106a_IntIsr()
{
    static uint8_t num = 0;
    distence_buffer[num] = LIB_UART_GetBlockData(CX20106A_UART);
    if(distence_buffer[0] != 0xa5) num = 0;                                                    //检查第一个数据是否为0xa5
    else
    {
        num++;
        distance_ok = 0x00;
    }

    if(num == 3)                                                                    //接收完成 开始处理数据
    {
        num = 0;
        distance = (uint16_t)distence_buffer[1]<<8 | (uint16_t)distence_buffer[2];                                       //得到超声波模块测出的距离
        if(distance < 50) distance = 50;
        if(distance > 6000) distance = 6000;
        distance_ok = 0x01;
    }
}

void Cx20106a_Init()
{
    LIB_UART_Init(CX20106A_UART, 115200, Cx20106a_IntIsr);
}

uint16_t Cx20106a_GetDistance()
{
    while(distance_ok == 0) ;
    distance_ok = 0;
    return distance;
}
