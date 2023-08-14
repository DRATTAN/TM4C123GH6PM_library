/*
 * timer.h
 *
 *  Created on: 2023/05/20
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_TIMER_LIB_TIMER_H_
#define EK_TM4C123GXL_LIB_TIMER_LIB_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "lib_timer_define.h"
#include "lib_isrmanagement.h"

#ifdef __cplusplus
extern "C" {
#endif

void LIB_TIMER_InitCycle(uint8_t, uint32_t, void (*)(void));
void LIB_TIMER_IntDisable(uint8_t);
void LIB_TIMER_IntEnable(uint8_t);
void LIB_TIMER_SetFreq(uint8_t, uint32_t);
void LIB_TIMER_TimerDisable(uint8_t);
void LIB_TIMER_TimerEnable(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* EK_TM4C123GXL_LIB_TIMER_LIB_TIMER_H_ */
