#include "LIB_INCLUDE.h"
#include "PERI_INCLUDE.h"
#include "STRUCT_INCLUDE.h"

int main(void)
{
    IntMasterEnable();
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M);
    LIB_UART_PrintfInit(0);
    while(1)
    {
        UARTprintf("HELLO");
        Oled_ShowParam(0, 0, "Param_v:", 100);
        delay_ms(10);
    }
}
