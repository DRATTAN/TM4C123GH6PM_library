/*
 * default.c
 *
 *  Created on: 2023/07/17
 *      Author: RATTAN
 *  ����չʾ����һ����򵥵ĳ�ʼ������
 */

#include "LIB_INCLUDE.h"        //MCU�����
#include "PERI_INCLUDE.h"       //����ģ���
#include "STRUCT_INCLUDE.h"     //���ݽṹ��

int main(void)
{
    LIB_GPIO_UnlockAllPins();                   //��������Ĭ������������
    IntMasterEnable();                          //�����ж�
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M); //����ϵͳʱ��
    while(1)    //������ѭ��
    {

    }
}

