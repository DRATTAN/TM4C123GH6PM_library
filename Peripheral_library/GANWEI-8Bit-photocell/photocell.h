/*
 * photocell.h
 *
 *  Created on: 2023/07/11
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_GANWEI_8BIT_PHOTOCELL_PHOTOCELL_H_
#define PERIPHERAL_LIB_GANWEI_8BIT_PHOTOCELL_PHOTOCELL_H_

#include "lib_i2c.h"

#define GW_GRAY_ADDR_DEF    0x9E >> 1
#define GW_GRAY_PING        0xAA
#define GW_GRAY_PING_OK     0x66
#define I2c_Addr            I2C0

uint8_t Photocell_Check(void);
uint8_t Photocell_ReadDigitalValue(void);
void Photocell_ReadAnalogValue(uint8_t *);


#endif /* PERIPHERAL_LIB_GANWEI_8BIT_PHOTOCELL_PHOTOCELL_H_ */
