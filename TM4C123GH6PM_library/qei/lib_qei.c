/*
 * lib_qei.c
 *
 *  Created on: 2023/05/24
 *      Author: RATTAN
 */

#include "lib_qei.h"
/*
 * 描述:初始化QEI模块
 * 参数
 *  @QEI_Num:
 *      QEI模块号:
 *      QEI0,QEI1
 *          QEIx    PHASE   PIN
 *          QEI0      A     PD6
 *          QEI0      B     PD7
 *
 *          QEI1      A     PC5
 *          QEI1      B     PC6
 *  @Freq:
 *      QEI中断触发频率
 *  @*QEIx_ISRhandle:
 *      QEI中断服务函数
 * 返回值:void
 * 备注:两个QEI模块可以从支持2个正交编码器,编码器一个周期捕获4个边缘.
 */
void LIB_QEI_Init(uint8_t QEI_Num, uint32_t Freq, void (*QEIx_ISRhandle)(void))
{
    uint32_t QEI_INT_TABLE[2] = {INT_QEI0, INT_QEI1};
    switch(QEI_Num)
    {
    case QEI0:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOD);
        GPIOUnlockPin(GPIO_PORTD_BASE, GPIO_PIN_7);
        GPIOPinConfigure(GPIO_PD6_PHA0);
        GPIOPinConfigure(GPIO_PD7_PHB0);
        GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
        break;
    case QEI1:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOC);
        GPIOPinConfigure(GPIO_PC5_PHA1);
        GPIOPinConfigure(GPIO_PC6_PHB1);
        GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
        GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
        break;
    default:
        break;

    }
    LIB_ISR_QEIRegister(QEI_Num, QEIx_ISRhandle);
    LIB_CLOCK_PeripheralClockEnable(QEI_Num + SYSCTL_PERIPH_QEI_BASE);
    QEIConfigure(((QEI_Num << 12) + QEI_BASE),(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 20000);
    QEIVelocityConfigure(((QEI_Num << 12) + QEI_BASE), QEI_VELDIV_1, SysCtlClockGet() / Freq - 1);
    QEIVelocityEnable(((QEI_Num << 12) + QEI_BASE));
    QEIIntEnable(((QEI_Num << 12) + QEI_BASE), QEI_INTTIMER);
    IntEnable(QEI_INT_TABLE[QEI_Num]);
    QEIEnable(((QEI_Num << 12) + QEI_BASE));
}
/*
 * 描述:获取速度计算器的数据
 * 参数
 *  @QEI_Num:
 *      QEI模块号:
 *          QEI0,QEI1
 * 返回值:
 *      uint32_t: 速度计算器的值
 * 备注:由于是双边沿计数,所以编码器的真实值应该是速度计算器的值除以4
 */
uint32_t LIB_QEI_GetPulse(uint8_t QEI_Num)
{
    return QEIVelocityGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * 描述:获取方向
 * 参数
 *  @QEI_Num:
 *      QEI模块号:
 *          QEI0,QEI1
 * 返回值:
 *      int8_t: 方向码,只可能为1或-1
 * 备注:方向是由编码器本身,A B相连接方式,电机旋转方向共同决定的,所以是一个相对量.
 * 故1或-1并不代表绝对的正转或反转,正反转是由观测者定义的,当观测者确定了编码器以及接线后
 * 方向码便和编码器旋转方向绑定,此时可以定义其中一个值为"正向",另一个个为"反向"(bushi)
 */
int8_t LIB_QEI_GetDirect(uint8_t QEI_Num)
{
    return (int8_t)QEIDirectionGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * 描述:获取绝对编码值
 * 参数
 *  @QEI_Num:
 *      QEI模块号:
 *          QEI0,QEI1
 * 返回值:
 *      uint32_t: 坐标值
 * 备注:由于是双边沿计数,所以编码器的真实值应该是绝对编码值除以4.绝对坐标最大积分值是0xFFFFFFFFU,在此之前应及时减小或清空积分器的值
 */
uint32_t LIB_QEI_GetPos(uint8_t QEI_Num)
{
    return QEIPositionGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * 描述:设置绝对编码值
 * 参数
 *  @QEI_Num:
 *      QEI模块号:
 *          QEI0,QEI1
 *          * 参数
 *  @Pos:
 *      要设置的值
 * 返回值:void
 * 备注:None
 */
void LIB_QEI_SetPos(uint8_t QEI_Num,uint32_t Pos)
{
    QEIPositionSet(((QEI_Num << 12) + QEI_BASE), Pos);
}
