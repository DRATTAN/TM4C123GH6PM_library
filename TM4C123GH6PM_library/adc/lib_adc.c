/*
 * lib_adc.c
 *
 *  Created on: 2023/07/19
 *      Author: RATTAN
 */

#include "lib_adc.h"
extern volatile uint8_t AdcSequenceSamplingFlage;
extern volatile uint32_t AdcSequenceSamplingValueBuffer[8];


/*
 * 描述:ADC初始化
 *          CHn     PORTPIN
 *          0       PE3
 *          1       PE2
 *          2       PE1
 *          3       PE0
 *          4       PC3
 *          5       PC2
 *          6       PC1
 *          7       PC0
 * 参数
 *  @SwitchByte:
 *      通道号,开启对应位上的通道:
 *              0x01-0xFF,最低位为CH0,最高位为CH7,当某一位置1时,该通道打开,否则关闭
 * 返回值:void
 * 备注:None
 */
void LIB_ADC_Init(uint8_t SwitchByte)
{
    const uint32_t ADC_INIT_TABLE[8][3] =
    {{SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_3},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_1},
     {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_0},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_3},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_2},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_1},
     {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, GPIO_PIN_0}};
    LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_ADC0);
    if(SwitchByte == 0x00) return;
    for(uint8_t i = 0; i < 8; i++)
    {
        if(((SwitchByte >> i) & 0x01) == 0x01)
        {
            LIB_CLOCK_PeripheralClockEnable(ADC_INIT_TABLE[i][0]);
            GPIOPinTypeADC(ADC_INIT_TABLE[i][1], ADC_INIT_TABLE[i][2]);
        }
    }
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH5 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH6 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);
    IntEnable(INT_ADC0SS0);
    ADCSequenceEnable(ADC0_BASE, 0);

}
/*
 * 描述:ADC读取值
 * 参数
 *  @DataBuffer:
 *          用来存储ADC转换结果的存储区,长度至少为8 个 uint16_t
 * 返回值:void
 * 备注:初始化时打开哪几个通道就读取哪几个通道,未打开的通道也会有值,但此值没有任何意义
 */
void LIB_ADC_GetValue(uint16_t *DataBuffer)
{
    AdcSequenceSamplingFlage = 0;
    ADCIntClear(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 0);
    while(AdcSequenceSamplingFlage == 0) ;
    for(uint8_t i = 0; i < 8; i++) DataBuffer[i] = (uint16_t)AdcSequenceSamplingValueBuffer[i];
    return ;
}
