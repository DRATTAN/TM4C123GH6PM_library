/*
 * hc-sr04.c
 *
 *  Created on: 2023/07/09
 *      Author: RATTAN
 */

#include "hc-sr04.h"

static uint32_t EchoPort = 0,EchoPin = 0, TrigPort = 0, TrigPin = 0;
static volatile uint8_t flag = 0;
static uint32_t p1 = 0;
static uint32_t p2 = 0;
/*
 * ����:�ڲ�����,�������
 * ����:Void
 * ����ֵ:void
 * ��ע:
 */
static void Hcsr04_IntIsr(void)
{
    if(flag == 0)
    {
        flag = 1;
        p1 = TimerValueGet(WTIMER0_BASE, TIMER_A);
        return ;
    }
    if(flag == 1)
    {
        flag = 2;
        p2 = TimerValueGet(WTIMER0_BASE, TIMER_A);
        LIB_GPIO_ExtiCtrlPin(EchoPort, EchoPin, GPIO_EXTI_CTRL_DISABLE);
        TimerLoadSet(WTIMER0_BASE, TIMER_A, 0);
        return ;
    }

}

/*
 * ����:�ڲ�����,�������
 * ����:Void
 * ����ֵ:void
 * ��ע:
 */
static void HcSr04_EchoInit()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    TimerClockSourceSet(WTIMER0_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT_UP);
    TimerLoadSet(WTIMER0_BASE, TIMER_A, SysCtlClockGet() * 0.015f);
}

/*
 * ����:�ڲ�����,�������
 * ����:Void
 * ����ֵ:void
 * ��ע:
 */
static void HcSr04_Trig(void)
{
    LIB_GPIO_WritePin(TrigPort, TrigPin, GPIO_PIN_SET);
    delay_us(15);
    LIB_GPIO_WritePin(TrigPort, TrigPin, GPIO_PIN_RESET);
}

/*
 * ����:��ʼ�����������ģ��
 * ����:
 *      @EISRPort:
 *          Echo���������ӵ��ж϶˿�
 *      @EISRPin
 *          Echo���������ӵ��ж�Pin
*       @TPort
*           trig���������ӵĶ˿�
*       @TPin
*           Echo���������ӵ�Pin
 * ����ֵ:void
 * ��ע:���� Hcsr04_Init(ISR_GPIOB, ISR_GPIO_PIN_3, GPIOB, GPIO_PIN_2); ע��:EISRPort��EISRPinһ����ISR_��ͷ
 */
void Hcsr04_Init(uint8_t EISRPort, uint8_t EISRPin, uint32_t TPort, uint32_t TPin)
{
    TrigPort = TPort;
    TrigPin = TPin;
    EchoPort = EISRPort;
    EchoPin = EISRPin;
    HcSr04_EchoInit();
    LIB_GPIO_ExtiInit(EISRPort, EISRPin, GPIO_EXTI_TRIGGER_BOTHEDGES, Hcsr04_IntIsr);
    LIB_GPIO_Init(TPort, TPin, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_RESET);
    delay_ms(1);
}

/*
 * ����:��ȡ����
 * ����:Void
 * ����ֵ:uint16_t:
 *      �õ��ľ���,��λmm
 * ��ע:�˺������ĵ�ʱ���������������,����������ʱ,�䲻�ᳬ��20����
 */
uint16_t Hcsr04_GetDistance(void)
{
    uint16_t dis = 0;
    flag = 0;
    LIB_GPIO_ExtiCtrlPin(EchoPort, EchoPin, GPIO_EXTI_CTRL_ENABLE);
    HcSr04_Trig();
    TimerLoadSet(WTIMER0_BASE, TIMER_A, SysCtlClockGet() * 0.015f);
    TimerEnable(WTIMER0_BASE, TIMER_A);
    while(flag != 2) ;
    if(p2 < p1 ) return 4000;
    dis = (float)(p2 - p1) * (float)190000.0f / (float)SysCtlClockGet() - 14.0f;
    if(dis > 4000) return 4000;
    return dis;
}

