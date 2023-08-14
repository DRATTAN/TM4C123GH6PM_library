/*EK-TM4C123GXL_LIB
 * 作者:RATTAN
 * 文件描述:none
 */

#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "lib_clock_Define.h"
#include "driverlib/systick.h"

#ifdef __cplusplus
extern "C" {
#endif


#define LIB_CLOCK_SysTickGet SysTickValueGet
extern volatile uint32_t CPU_F;


uint32_t LIB_CLOCK_MainClockSet(uint32_t);
void LIB_CLOCK_PeripheralClockEnable(uint32_t);
void LIB_CLOCK_PeripheralClockDisable(uint32_t);


#ifdef __cplusplus
}
#endif

#endif
