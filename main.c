#include "LIB_INCLUDE.h"        //MCU�����
#include "PERI_INCLUDE.h"       //����ģ���
#include "STRUCT_INCLUDE.h"     //���ݽṹ��

int main(void)
{
    IntMasterEnable();
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M);
    LIB_UART_PrintfInit(0);
    LIB_GPIO_Init(GPIOF, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_SET);
    while(1)
    {
        LIB_GPIO_TogglePin(GPIOF, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);
        delay_ms(100);
    }
}
