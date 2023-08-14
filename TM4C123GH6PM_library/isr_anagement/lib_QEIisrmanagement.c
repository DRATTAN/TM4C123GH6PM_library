/*
 * lib_QEIisrmanagement.c
 *
 *  Created on: 2023/05/25
 *      Author: RATTAN
 */

#include "lib_isrmanagement.h"

void (* volatile QeiIsrHandleTable[2])(void) = {__nop, __nop};

void QEI0_ISR_Handle(void)
{
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
    (*QeiIsrHandleTable[0])();
}

void QEI1_ISR_Handle(void)
{
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);
    (*QeiIsrHandleTable[1])();
}

void LIB_ISR_QEIRegister(uint8_t QEI_Base, void (*ISR_handler)(void))
{
    QeiIsrHandleTable[QEI_Base] = ISR_handler;
    return ;
}
