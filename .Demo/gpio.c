 /* gpio.c
 *
 *  Created on: 2023/07/17
 *      Author: RATTAN
 *  这里展示的是一个gpio使用流程
 */

#include "LIB_INCLUDE.h"
#include "PERI_INCLUDE.h"
#include "STRUCT_INCLUDE.h"

void toggle(void);

int main(void)
{
    LIB_GPIO_UnlockAllPins();                   //解锁所有默认锁定的引脚
    IntMasterEnable();                          //打开总中断

    //此函数会配置串口0作为调试串口,波特率115200,通过MicroUSB线直接与PC连接,在
    //PC上产生一个虚拟串口,通过串口助手可以接收信息
    LIB_UART_PrintfInit();

    //初始化PF1(板载红灯)作为观察对象,此灯高电平亮,低电平灭
    LIB_GPIO_Init(GPIOF, GPIO_PIN_1, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_RESET);

    //初始化PF0(板载SW2)作为外部中断触发按键,此按键为按下后接地,所以将中断配置为下降沿触发,
    //注册中断函数为toggle
    LIB_GPIO_ExtiInit(ISR_GPIOF, ISR_GPIO_PIN_0, GPIO_EXTI_TRIGGER_FALLING, toggle);
    while(1)
    {
        delay_ms(500);
    }
}


void toggle()
{
    delay_ms(100);//消抖

    //这里会翻转PF1的电平
    LIB_GPIO_TogglePin(GPIOF, GPIO_PIN_1);
}