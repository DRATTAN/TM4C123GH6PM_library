/*EK-TM4C123GXL_LIB
 * 作者:RATTAN
 * 文件描述:none
 */


#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stdbool.h>

#include "lib_clock.h"
#include "lib_gpio_define.h"
#include "lib_isrmanagement.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#ifdef __cplusplus
extern "C" {
#endif

void LIB_GPIO_UnlockAllPins(void);
void LIB_GPIO_Init(uint32_t, uint8_t, uint8_t, uint32_t, uint8_t);
void LIB_GPIO_WritePin(uint32_t, uint32_t, uint8_t);
uint8_t LIB_GPIO_ReadPin(uint32_t, uint32_t);
void LIB_GPIO_TogglePin(uint32_t, uint32_t);
void LIB_GPIO_ExtiInit(uint8_t, uint8_t, uint8_t, void (*)(void));
void LIB_GPIO_ExtiCtrlPin(uint8_t, uint8_t, uint8_t);
void LIB_GPIO_ExtiCtrlPort(uint8_t, uint8_t);
void LIB_GPIO_ExtiIsrSet(uint8_t, uint8_t, void (*)(void));

#ifdef __cplusplus
}
#endif

#endif
