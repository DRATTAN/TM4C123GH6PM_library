/*
 * lib_TIMERisrmanagement.c
 *
 *  Created on: 2023/05/20
 *      Author: RATTAN
 */

#include "lib_isrmanagement.h"

void (* volatile TimerIsrHandleTable[6][2])(void)  =
{
 {__nop, __nop},
 {__nop, __nop},
 {__nop, __nop},
 {__nop, __nop},
 {__nop, __nop},
 {__nop, __nop},
};

void TIMER0A_ISR_Handle(void)
{
    uint32_t status  =  TimerIntStatus(TIMER0_BASE, true);
    TimerIntClear(TIMER0_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 0, 0)();
}

void TIMER0B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER0_BASE, true);
    TimerIntClear(TIMER0_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 0, 1)();
}

void TIMER1A_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER1_BASE, true);
    TimerIntClear(TIMER1_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 1, 0)();
}

void TIMER1B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER1_BASE, true);
    TimerIntClear(TIMER1_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 1, 1)();
}

void TIMER2A_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER2_BASE, true);
    TimerIntClear(TIMER2_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 2, 0)();
}

void TIMER2B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER2_BASE, true);
    TimerIntClear(TIMER2_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 2, 1)();
}

void TIMER3A_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER3_BASE, true);
    TimerIntClear(TIMER3_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 3, 0)();
}

void TIMER3B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER3_BASE, true);
    TimerIntClear(TIMER3_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 3, 1)();
}

void TIMER4A_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER4_BASE, true);
    TimerIntClear(TIMER4_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 4, 0)();
}

void TIMER4B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER4_BASE, true);
    TimerIntClear(TIMER4_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 4, 1)();
}

void TIMER5A_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER5_BASE, true);
    TimerIntClear(TIMER5_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 5, 0)();
}

void TIMER5B_ISR_Handle(void)
{
    uint32_t status = TimerIntStatus(TIMER5_BASE, true);
    TimerIntClear(TIMER5_BASE, status);
    ACCESS_ISR_TABLE(TimerIsrHandleTable, 5, 1)();
}

void LIB_ISR_TIMERISRInit()
{
    TimerIntRegister(TIMER0_BASE,  TIMER_A, TIMER0A_ISR_Handle);
    TimerIntRegister(TIMER0_BASE,  TIMER_B, TIMER0B_ISR_Handle);
    TimerIntRegister(TIMER1_BASE,  TIMER_A, TIMER1A_ISR_Handle);
    TimerIntRegister(TIMER1_BASE,  TIMER_B, TIMER1B_ISR_Handle);
    TimerIntRegister(TIMER2_BASE,  TIMER_A, TIMER2A_ISR_Handle);
    TimerIntRegister(TIMER2_BASE,  TIMER_B, TIMER2B_ISR_Handle);
    TimerIntRegister(TIMER3_BASE,  TIMER_A, TIMER3A_ISR_Handle);
    TimerIntRegister(TIMER3_BASE,  TIMER_B, TIMER3B_ISR_Handle);
    TimerIntRegister(TIMER4_BASE,  TIMER_A, TIMER4A_ISR_Handle);
    TimerIntRegister(TIMER4_BASE,  TIMER_B, TIMER5B_ISR_Handle);
    TimerIntRegister(TIMER5_BASE,  TIMER_A, TIMER5A_ISR_Handle);
    TimerIntRegister(TIMER5_BASE,  TIMER_B, TIMER5B_ISR_Handle);
}

void LIB_ISR_TIMERRegister(uint8_t TIMER_Base, uint8_t TIMER_Part, void (*ISR_handler)(void))
{
    TimerIsrHandleTable[TIMER_Base][TIMER_Part] = ISR_handler;
    return ;
}
