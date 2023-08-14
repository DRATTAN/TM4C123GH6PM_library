/*
 * lib_adc.h
 *
 *  Created on: 2023/07/19
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_ADC_LIB_ADC_H_
#define EK_TM4C123GXL_LIB_ADC_LIB_ADC_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"


#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"

#include "lib_gpio.h"
#include  "lib_adc_define.h"

void LIB_ADC_Init(uint8_t);
void LIB_ADC_GetValue(uint16_t *);


#endif /* EK_TM4C123GXL_LIB_ADC_LIB_ADC_H_ */
