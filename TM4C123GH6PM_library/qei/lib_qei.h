/*
 * lib_qei.h
 *
 *  Created on: 2023/05/24
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_QEI_LIB_QEI_H_
#define EK_TM4C123GXL_LIB_QEI_LIB_QEI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"
#include "lib_qei_define.h"
#include "lib_isrmanagement.h"

#ifdef __cplusplus
extern "C" {
#endif

void LIB_QEI_Init(uint8_t, uint32_t, void (*)(void));
uint32_t LIB_QEI_GetPulse(uint8_t);
int8_t LIB_QEI_GetDirect(uint8_t);
uint32_t LIB_QEI_GetPos(uint8_t);
void LIB_QEI_SetPos(uint8_t,uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* EK_TM4C123GXL_LIB_QEI_LIB_QEI_H_ */
