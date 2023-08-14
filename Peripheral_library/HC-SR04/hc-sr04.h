/*
 * hc-sr04.h
 *
 *  Created on: 2023/07/09
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_HC_SR04_HC_SR04_H_
#define PERIPHERAL_LIB_HC_SR04_HC_SR04_H_

#include "lib_gpio.h"
#include "lib_timer.h"


void Hcsr04_Init(uint8_t, uint8_t, uint32_t, uint32_t);
uint16_t Hcsr04_GetDistance(void);
#endif /* PERIPHERAL_LIB_HC_SR04_HC_SR04_H_ */
