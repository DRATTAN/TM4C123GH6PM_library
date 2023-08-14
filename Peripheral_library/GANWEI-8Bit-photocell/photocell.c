/*
 * photocell.c
 *
 *  Created on: 2023/07/11
 *      Author: RATTAN
 */

#include "photocell.h"



uint8_t Photocell_Check()
{
    if(GW_GRAY_PING_OK == LIB_I2C_Get(I2c_Addr, GW_GRAY_ADDR_DEF, GW_GRAY_PING))  return 0;
    return 1;
}

uint8_t Photocell_ReadDigitalValue()
{
    return LIB_I2C_Get(I2c_Addr, GW_GRAY_ADDR_DEF, 0xDD);
}

void Photocell_ReadAnalogValue(uint8_t * BufferPoint)
{
    LIB_I2C_Recive(I2c_Addr, GW_GRAY_ADDR_DEF, 0xB0, BufferPoint, 8);
}
