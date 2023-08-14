/*
 * lib_time.h
 *
 *  Created on: 2023/05/19
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_TIME_LIB_TIME_H_
#define EK_TM4C123GXL_LIB_TIME_LIB_TIME_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "lib_clock.h"

//#define CPU_F ((double)80000000)

#ifdef __cplusplus
extern "C" {
#endif

#define delay_systick(n) SysCtlDelay(n)

#define delay_us(n) SysCtlDelay(n*(SysCtlClockGet()/3000000))

#define delay_ms(n) SysCtlDelay(n*(SysCtlClockGet()/3000))

#ifdef __cplusplus
}
#endif

#endif /* EK_TM4C123GXL_LIB_TIME_LIB_TIME_H_ */
