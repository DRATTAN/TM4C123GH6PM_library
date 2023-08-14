/*
 * tsl1401.h
 *
 *  Created on: 2023/07/25
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_TSL1401_TSL1401_H_
#define PERIPHERAL_LIB_TSL1401_TSL1401_H_

#include "lib_gpio.h"
#include "lib_adc.h"
#include "oled.h"
#include "string.h"

#define TSL1401_CLK_PORT GPIOF
#define TSL1401_CLK_PIN GPIO_PIN_1

#define TSL1401_SI_PORT GPIOF
#define TSL1401_SI_PIN GPIO_PIN_2

#define TSL1401_A0_CHANNEL 3

#define CLK_WRITE(x)   LIB_GPIO_WritePin(TSL1401_CLK_PORT, TSL1401_CLK_PIN, x);
#define SI_WRITE(x)   LIB_GPIO_WritePin(TSL1401_SI_PORT, TSL1401_SI_PIN, x);

void Tsl1401_Init(void);
void Tsl1401_GetData(uint16_t, uint8_t *);
//uint8_t Ts1401_GetThreshold(const uint8_t *);
void Tsl1401_Show(uint8_t, uint8_t, const uint8_t *);
void Ts1401_Trans(uint8_t *);
#endif /* PERIPHERAL_LIB_TSL1401_TSL1401_H_ */
