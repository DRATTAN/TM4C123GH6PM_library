/*
 * default.c
 *
 *  Created on: 2023/07/17
 *      Author: RATTAN
 *  这里展示的是一个最简单的初始化流程
 */

#include "LIB_INCLUDE.h"        //MCU外设库
#include "PERI_INCLUDE.h"       //常用模块库
#include "STRUCT_INCLUDE.h"     //数据结构库

int main(void)
{
    LIB_GPIO_UnlockAllPins();                   //解锁所有默认锁定的引脚
    IntMasterEnable();                          //打开总中断
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M); //设置系统时钟
    while(1)    //进入主循环
    {

    }
}

