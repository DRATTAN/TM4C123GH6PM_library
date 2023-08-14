/*
 * cx20106a.h
 *
 *  Created on: 2023/07/13
 *      Author: RATTAN
 */

#ifndef PERIPHERAL_LIB_CX20106A_CX20106A_H_
#define PERIPHERAL_LIB_CX20106A_CX20106A_H_

#include "lib_uart.h"

#define CX20106A_UART UART4

void Cx20106a_IntIsr();
void Cx20106a_Init();
uint16_t Cx20106a_GetDistance();

#endif /* PERIPHERAL_LIB_CX20106A_CX20106A_H_ */
