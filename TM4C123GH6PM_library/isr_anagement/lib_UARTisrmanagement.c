/*
 * lib_UARTisrmanagement.c
 *
 *  Created on: 2023/05/22
 *      Author: RATTAN
 */
#include "lib_isrmanagement.h"

void (* volatile UartIsrHandleTable[8])(void) = {__nop, __nop, __nop, __nop, __nop, __nop,__nop, __nop};

void UART0_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, Status);
    (*(*(UartIsrHandleTable + 0)))();
}

void UART1_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, Status);
    (*(*(UartIsrHandleTable + 1)))();
}

void UART2_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART2_BASE, true);
    UARTIntClear(UART2_BASE, Status);
    (*(*(UartIsrHandleTable + 2)))();
}

void UART3_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, Status);
    (*(*(UartIsrHandleTable + 3)))();
}

void UART4_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART4_BASE, true);
    UARTIntClear(UART4_BASE, Status);
    (*(*(UartIsrHandleTable + 4)))();
}

void UART5_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, Status);
    (*(*(UartIsrHandleTable + 5)))();
}

void UART6_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART6_BASE, true);
    UARTIntClear(UART6_BASE, Status);
    (*(*(UartIsrHandleTable + 6)))();
}

void UART7_ISR_Handle(void)
{
    uint32_t Status = UARTIntStatus(UART7_BASE, true);
    UARTIntClear(UART7_BASE, Status);
    (*(*(UartIsrHandleTable + 7)))();
}

void LIB_ISR_UARTISRInit()
{
    UARTIntRegister(UART0_BASE, UART0_ISR_Handle);
    UARTIntRegister(UART1_BASE, UART1_ISR_Handle);
    UARTIntRegister(UART2_BASE, UART2_ISR_Handle);
    UARTIntRegister(UART3_BASE, UART3_ISR_Handle);
    UARTIntRegister(UART4_BASE, UART4_ISR_Handle);
    UARTIntRegister(UART5_BASE, UART5_ISR_Handle);
    UARTIntRegister(UART6_BASE, UART6_ISR_Handle);
    UARTIntRegister(UART7_BASE, UART7_ISR_Handle);

}
void LIB_ISR_UARTRegister(uint8_t UART_Base, void (*ISR_handler)(void))
{
    UartIsrHandleTable[UART_Base] = ISR_handler;
    return ;
}
