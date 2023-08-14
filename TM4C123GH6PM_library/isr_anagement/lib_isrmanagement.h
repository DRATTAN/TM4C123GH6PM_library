#ifndef __LIB_ISRMANAGEMENT_H
#define __LIB_ISRMANAGEMENT_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "lib_gpio.h"
#include "lib_uart.h"
#include "lib_isrmanagement_define.h"

#define ACCESS_ISR_TABLE(TABLE, GROUP, RANK) (* (* (* (TABLE + GROUP) + RANK)))


#ifdef __cplusplus
extern "C" {
#endif

void __nop(void);

void LIB_ISR_GPIOEXTIISRInit(void);
void LIB_ISR_GPIOEXTIRegister(uint8_t, uint8_t, void (*)(void));

void LIB_ISR_TIMERISRInit(void);
void LIB_ISR_TIMERRegister(uint8_t, uint8_t, void (*)(void));

void LIB_ISR_UARTISRInit(void);
void LIB_ISR_UARTRegister(uint8_t, void (*)(void));

void LIB_ISR_QEIRegister(uint8_t, void (*)(void));

#ifdef __cplusplus
}
#endif

#endif
