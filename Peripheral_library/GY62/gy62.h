/*
 * gy62.h
 *
 *  Created on: 2023/07/23
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_GY62_GY62_H_
#define PERIPHERAL_LIB_GY62_GY62_H_

#include"lib_uart.h"

#define GY62_UART UART5


void Gy62_Init(void);
float Gy62_GetRoll(void);
float Gy62_GetPitch(void);
float Gy62_GetYaw(void);
void Gy62_GetAngle(float *);


#endif /* PERIPHERAL_LIB_GY62_GY62_H_ */
