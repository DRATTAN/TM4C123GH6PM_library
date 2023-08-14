/*
 * lib_pwm.h
 *
 *  Created on: 2023/05/26
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_PWM_LIB_PWM_H_
#define EK_TM4C123GXL_LIB_PWM_LIB_PWM_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"


#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"

#include "lib_pwm_define.h"
#include "lib_clock.h"


#ifdef __cplusplus
extern "C" {
#endif

void LIB_PWM_Init(uint8_t,uint8_t, uint32_t, uint16_t,uint16_t);
void LIB_PWM_SetPulseWidth(uint8_t, uint8_t, uint16_t);

#ifdef __cplusplus
}
#endif

#endif /* EK_TM4C123GXL_LIB_PWM_LIB_PWM_H_ */
