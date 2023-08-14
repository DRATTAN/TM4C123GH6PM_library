/*
 * default.c
 *
 *  Created on: 2023/07/17
 *      Author: RATTAN
 *  ����չʾ����һ��ADCʹ������
 */

#include "LIB_INCLUDE.h"        //MCU�����
#include "PERI_INCLUDE.h"       //����ģ���
#include "STRUCT_INCLUDE.h"     //���ݽṹ��

uint16_t ADCdata[8];//�洢ADC�ɼ����������

int main(void)
{
    LIB_GPIO_UnlockAllPins();                   //��������Ĭ������������
    IntMasterEnable();                          //�����ж�
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M); //����ϵͳʱ��

    //�˺��������ô���0��Ϊ���Դ���,������115200,ͨ��MicroUSB��ֱ����PC����,��
    //PC�ϲ���һ�����⴮��,ͨ���������ֿ��Խ�����Ϣ
    LIB_UART_PrintfInit();

    LIB_ADC_Init(0x03)//�����ʼ��ʱ����ͨ��0��ͨ��1

    while(1)    //������ѭ��
    {
        //���ɼ���Ϻ�,�ɼ������洢��ADCdata[8]��,��Ϊֻ����ͨ��0��1,����ֻ��
        //ADCdata[0]��ADCdata[1]����������Ч��,ʣ���������û���κ������,����Ҫ����ͨ��,����
        //LIB_ADC_Init(SwitchByte)�������,ע��,��Ҫ��һ���������ö��ֹ���
        LIB_ADC_GetValue(ADCdata); 
        UARTprintf("ADC CH0 data: %u \n ADC CH1 data: %u \n\n",ADCdata[0], ADCdata[1]);
    }
}