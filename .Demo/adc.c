/*
 * default.c
 *
 *  Created on: 2023/07/17
 *      Author: RATTAN
 *  这里展示的是一个ADC使用流程
 */

#include "LIB_INCLUDE.h"        //MCU外设库
#include "PERI_INCLUDE.h"       //常用模块库
#include "STRUCT_INCLUDE.h"     //数据结构库

uint16_t ADCdata[8];//存储ADC采集结果的数组

int main(void)
{
    LIB_GPIO_UnlockAllPins();                   //解锁所有默认锁定的引脚
    IntMasterEnable();                          //打开总中断
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M); //设置系统时钟

    //此函数会配置串口0作为调试串口,波特率115200,通过MicroUSB线直接与PC连接,在
    //PC上产生一个虚拟串口,通过串口助手可以接收信息
    LIB_UART_PrintfInit();

    LIB_ADC_Init(0x03)//这里初始化时打开了通道0和通道1

    while(1)    //进入主循环
    {
        //当采集完毕后,采集结果会存储在ADCdata[8]里,因为只打开了通道0和1,所以只有
        //ADCdata[0]和ADCdata[1]的数据是有效的,剩余的数据是没有任何意义的,若需要更多通道,请在
        //LIB_ADC_Init(SwitchByte)里打开他们,注意,不要将一个引脚配置多种功能
        LIB_ADC_GetValue(ADCdata); 
        UARTprintf("ADC CH0 data: %u \n ADC CH1 data: %u \n\n",ADCdata[0], ADCdata[1]);
    }
}