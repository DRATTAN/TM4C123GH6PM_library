/*EK-TM4C123GXL_LIB
 * 作者:RATTAN
 * 文件描述:none
 */

#include "lib_clock.h"

volatile uint32_t CPU_F = 0;

/*
 * 描述:设置系统时钟频率
 * 参数
 *  @DIV:
 *       CLOCK_XTAL_PLL_80M:设置系统时钟为80HMz
 *       CLOCK_XTAL_PLL_40M:设置系统时钟为40HMz
 *       CLOCK_XTAL_PLL_20M:设置系统时钟为20HMz
 *       CLOCK_XTAL_PLL_10M:设置系统时钟为10HMz
 *       CLOCK_XTAL_PLL_5M:设置系统时钟为5HMz
 * 返回值:uint32_t 系统时钟频率
 * 备注:对于TM4C123GXL,系统时钟不可超过80MHz
 */
uint32_t LIB_CLOCK_MainClockSet(uint32_t DIV)
{
    SysCtlClockSet(DIV | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    CPU_F = SysCtlClockGet();
    return CPU_F;
}

/*
 * 描述:使能外设时钟并等待其配置完成
 * 参数
 *  @PeripheralAddr:
 *       外设基地址,例如:
 *       GPIO_PORTB_AHB_BASE
 *       GPIO_PORTB_BASE
 * 返回值:void
 * 备注:若时钟未初始化完成,则禁止操作总线,否则会引起故障.若1ms内时钟未初始化完成,则检查程序或硬件.
 */
void LIB_CLOCK_PeripheralClockEnable(uint32_t PeripheralAddr)
{
    SysCtlPeripheralEnable(PeripheralAddr);
    while(!SysCtlPeripheralReady(PeripheralAddr)) ;
    return;
}

/*
 * 描述:关闭外设时钟并等待其配置完成
 * 参数
 *  @PeripheralAddr:
 *       外设基地址,例如:
 *       GPIO_PORTB_AHB_BASE
 *       GPIO_PORTB_BASE
 * 返回值:void
 * 备注:若时钟执行关闭后未关闭完成,则禁止操作总线,否则会引起故障.若1ms内时钟未关闭完成,则检查程序或硬件.
 */
void LIB_CLOCK_PeripheralClockDisable(uint32_t ui32Peripheral)
{
    SysCtlPeripheralDisable(ui32Peripheral);
    while(SysCtlPeripheralReady(ui32Peripheral)) ;
    return;
}
