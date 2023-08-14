/*
 * lib_pwm.c
 *
 *  Created on: 2023/05/26
 *      Author: RATTAN
 */

#include "lib_pwm.h"

#define LIB_PWM_Gen_Perio_Table_Set(PWm_Num, PWM_Chanel_Base, Freq) GEN_PERIO_TABLE[PWm_Num][PWM_Chanel_Base/2] = Freq
#define LIB_PWM_Gen_Perio_Table_Get(PWm_Num, PWM_Chanel_Base) GEN_PERIO_TABLE[PWm_Num][PWM_Chanel_Base/2]

static const uint32_t PWM_OUT_TABLE[8] = {PWM_OUT_0, PWM_OUT_1, PWM_OUT_2, PWM_OUT_3, PWM_OUT_4, PWM_OUT_5, PWM_OUT_6, PWM_OUT_7};
static uint32_t PWM_OUT_BIT_TABLE[8] = {PWM_OUT_0_BIT, PWM_OUT_1_BIT, PWM_OUT_2_BIT, PWM_OUT_3_BIT, PWM_OUT_4_BIT, PWM_OUT_5_BIT, PWM_OUT_6_BIT, PWM_OUT_7_BIT};


/*
 * 描述:初始化PWM模块
 * PWM_NUM  PWM_CHANNEL     PIN
 *      0       0           B6
 *      0       1           B7
 *      0       2           B4
 *      0       3           B5
 *      0       4           E4
 *      0       5           E5
 *      0       6           C4
 *      0       7           C5
 *
 *      1       0           D0
 *      1       1           D1
 *      1       2           A6
 *      1       3           A7
 *      1       4           F0
 *      1       5           F1
 *      1       6           F2
 *      1       7           F3
 *
 * 参数
 *  @PWm_Num:
 *      PWM 模块号 PWM_NUM_0 , PWM_NUM_1
 *  @PWM_Gen_Num:
 *      PWM 发生器号 PWM_GEN_NUM_0 - PWM_GEN_NUM_4
 *  @Freq:
 *      pwm频率 40 - 65535
 *  @dutyA:
 *      A通道初始占空比
 *  @dutyB:
 *      B通道初始占空比
 * 返回值:void
 * 备注:PWM满占空比为 主频 / 32 / Freq - 1
 */
void LIB_PWM_Init(uint8_t PWm_Num,uint8_t PWM_Gen_Num, uint32_t Freq, uint16_t dutyA,uint16_t dutyB)
{
    uint32_t GPIO_PORT_TABLE[2][4] = {{GPIO_PORTB_BASE, GPIO_PORTB_BASE, GPIO_PORTE_BASE, GPIO_PORTC_BASE},{GPIO_PORTD_BASE, GPIO_PORTA_BASE, GPIO_PORTF_BASE,GPIO_PORTF_BASE}};
    uint32_t GPIO_PIN_TABLE[2][4] = {{GPIO_PIN_6, GPIO_PIN_4, GPIO_PIN_4, GPIO_PIN_4},{GPIO_PIN_0, GPIO_PIN_6, GPIO_PIN_0, GPIO_PIN_2}};
    uint32_t GPIO_PERIPH_TABLE[2][4] = {{SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_GPIOE, SYSCTL_PERIPH_GPIOC},{SYSCTL_PERIPH_GPIOD, SYSCTL_PERIPH_GPIOA, SYSCTL_PERIPH_GPIOF,SYSCTL_PERIPH_GPIOF}};
    uint32_t GPIO_Px_MXPWMx_TABLE[2][4] = {{GPIO_PB6_M0PWM0, GPIO_PB4_M0PWM2, GPIO_PE4_M0PWM4, GPIO_PC4_M0PWM6},{GPIO_PD0_M1PWM0, GPIO_PA6_M1PWM2, GPIO_PF0_M1PWM4, GPIO_PF2_M1PWM6}};

    SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
    LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_PWM_BASE + PWm_Num);
    LIB_CLOCK_PeripheralClockEnable(GPIO_PERIPH_TABLE[PWm_Num][PWM_Gen_Num]);
    GPIOPinConfigure(GPIO_Px_MXPWMx_TABLE[PWm_Num][PWM_Gen_Num]);
    GPIOPinConfigure(GPIO_Px_MXPWMx_TABLE[PWm_Num][PWM_Gen_Num] + (0x00000004 << 8));
    GPIOPinTypePWM(GPIO_PORT_TABLE[PWm_Num][PWM_Gen_Num], GPIO_PIN_TABLE[PWm_Num][PWM_Gen_Num] | (GPIO_PIN_TABLE[PWm_Num][PWM_Gen_Num] << 1));
    PWMGenConfigure((PWM_BASE + (PWm_Num << 12)), (PWM_GEN_BASE + ((0x00000004 * PWM_Gen_Num) << 4)),PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet((PWM_BASE + (PWm_Num << 12)), (PWM_GEN_BASE + ((0x00000004 * PWM_Gen_Num) << 4)), SysCtlClockGet() / 32 / Freq - 1);
    LIB_PWM_SetPulseWidth(PWm_Num, PWM_Gen_Num * 2, dutyA);
    LIB_PWM_SetPulseWidth(PWm_Num, PWM_Gen_Num * 2 + 1, dutyB);
    PWMGenEnable((PWM_BASE + (PWm_Num << 12)), (PWM_GEN_BASE + ((0x00000004 * PWM_Gen_Num) << 4)));
}

/*
 * 描述:设置占空比
 * 参数
 *  @PWm_Num:
 *      PWM 模块号 PWM_NUM_0 , PWM_NUM_1
 *  @PWM_Channel_Num:
 *      PWM 通道号 PWM_CHANNEL_NUM_0 - PWM_CHANNEL_NUM_7
 *  @duty:
 *      占空比
 * 返回值:void
 * 备注: PWM满占空比为 主频 / 32 / Freq - 1
 */
void LIB_PWM_SetPulseWidth(uint8_t PWm_Num, uint8_t PWM_Channel_Num,uint16_t duty)
{
    if(duty != 0)
    {
        PWMPulseWidthSet((PWM_BASE + (PWm_Num << 12)), PWM_OUT_TABLE[PWM_Channel_Num], duty);
        PWMOutputState((PWM_BASE + (PWm_Num << 12)),PWM_OUT_BIT_TABLE[PWM_Channel_Num], duty);
    }

    else
    {
        PWMOutputState((PWM_BASE + (PWm_Num << 12)),PWM_OUT_BIT_TABLE[PWM_Channel_Num], duty);
        PWMPulseWidthSet((PWM_BASE + (PWm_Num << 12)), PWM_OUT_TABLE[PWM_Channel_Num], duty);
    }

}

