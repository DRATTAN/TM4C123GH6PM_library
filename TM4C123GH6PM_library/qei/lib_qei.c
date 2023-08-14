/*
 * lib_qei.c
 *
 *  Created on: 2023/05/24
 *      Author: RATTAN
 */

#include "lib_qei.h"
/*
 * ����:��ʼ��QEIģ��
 * ����
 *  @QEI_Num:
 *      QEIģ���:
 *      QEI0,QEI1
 *          QEIx    PHASE   PIN
 *          QEI0      A     PD6
 *          QEI0      B     PD7
 *
 *          QEI1      A     PC5
 *          QEI1      B     PC6
 *  @Freq:
 *      QEI�жϴ���Ƶ��
 *  @*QEIx_ISRhandle:
 *      QEI�жϷ�����
 * ����ֵ:void
 * ��ע:����QEIģ����Դ�֧��2������������,������һ�����ڲ���4����Ե.
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
 * ����:��ȡ�ٶȼ�����������
 * ����
 *  @QEI_Num:
 *      QEIģ���:
 *          QEI0,QEI1
 * ����ֵ:
 *      uint32_t: �ٶȼ�������ֵ
 * ��ע:������˫���ؼ���,���Ա���������ʵֵӦ�����ٶȼ�������ֵ����4
 */
uint32_t LIB_QEI_GetPulse(uint8_t QEI_Num)
{
    return QEIVelocityGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * ����:��ȡ����
 * ����
 *  @QEI_Num:
 *      QEIģ���:
 *          QEI0,QEI1
 * ����ֵ:
 *      int8_t: ������,ֻ����Ϊ1��-1
 * ��ע:�������ɱ���������,A B�����ӷ�ʽ,�����ת����ͬ������,������һ�������.
 * ��1��-1����������Ե���ת��ת,����ת���ɹ۲��߶����,���۲���ȷ���˱������Լ����ߺ�
 * �������ͱ�������ת�����,��ʱ���Զ�������һ��ֵΪ"����",��һ����Ϊ"����"(bushi)
 */
int8_t LIB_QEI_GetDirect(uint8_t QEI_Num)
{
    return (int8_t)QEIDirectionGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * ����:��ȡ���Ա���ֵ
 * ����
 *  @QEI_Num:
 *      QEIģ���:
 *          QEI0,QEI1
 * ����ֵ:
 *      uint32_t: ����ֵ
 * ��ע:������˫���ؼ���,���Ա���������ʵֵӦ���Ǿ��Ա���ֵ����4.��������������ֵ��0xFFFFFFFFU,�ڴ�֮ǰӦ��ʱ��С����ջ�������ֵ
 */
uint32_t LIB_QEI_GetPos(uint8_t QEI_Num)
{
    return QEIPositionGet(((QEI_Num << 12) + QEI_BASE));
}

/*
 * ����:���þ��Ա���ֵ
 * ����
 *  @QEI_Num:
 *      QEIģ���:
 *          QEI0,QEI1
 *          * ����
 *  @Pos:
 *      Ҫ���õ�ֵ
 * ����ֵ:void
 * ��ע:None
 */
void LIB_QEI_SetPos(uint8_t QEI_Num,uint32_t Pos)
{
    QEIPositionSet(((QEI_Num << 12) + QEI_BASE), Pos);
}
