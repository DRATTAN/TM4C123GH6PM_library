/*
 * lib_i2c.h
 *
 *  Created on: 2023/05/27
 *      Author: RATTAN
 */

#ifndef EK_TM4C123GXL_LIB_I2C_LIB_I2C_H_
#define EK_TM4C123GXL_LIB_I2C_LIB_I2C_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_i2c.h"

#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"


#include "lib_clock.h"
#include "lib_isrmanagement.h"
#include "lib_i2c_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define timeout 500

void LIB_I2C_Init(uint8_t, uint8_t);
void LIB_I2C_Put(uint8_t, uint16_t, uint16_t, uint8_t);
void LIB_I2C_Transmit(uint8_t, uint16_t, uint16_t, const uint8_t*, uint32_t);
uint8_t LIB_I2C_Get(uint8_t, uint16_t, uint16_t);
void LIB_I2C_Recive(uint8_t, uint16_t, uint16_t, uint8_t*, uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* EK_TM4C123GXL_LIB_I2C_LIB_I2C_H_ */
