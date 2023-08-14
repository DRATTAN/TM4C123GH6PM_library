/*
 * gy62.c
 *
 *  Created on: 2023/07/23
 *      Author: RATTAN
 */
#include "gy62.h"

static uint8_t buffer[10];
static volatile uint8_t flags = 0;
static volatile float Roll = 0.0f;
static volatile float Pitch = 0.0f;
static volatile float Yaw = 0.0f;

static void Gy62_GetDigUartInitIsr(void);

void Gy62_Init()
{
    LIB_UART_Init(GY62_UART, 115200, Gy62_GetDigUartInitIsr);
}


float Gy62_GetRoll()
{
    flags = 0x00;
    while(flags == 0x00) __nop();
    return Roll;
}

float Gy62_GetPitch()
{
    flags = 0x00;
    while(flags == 0x00) __nop();
    return Pitch;
}

float Gy62_GetYaw()
{
    flags = 0x00;
    while(flags == 0x00) __nop();
    return Yaw;
}


void Gy62_GetAngle(float* Buffer)
{
    flags = 0x00;
    while(flags == 0x00) __nop();
    *(Buffer + 0) = Roll;
    *(Buffer + 1) = Pitch;
    *(Buffer + 2) = Yaw;
}

static void Gy62_GetDigUartInitIsr()
{
    static uint8_t counter = 0;
    buffer[counter] = LIB_UART_GetBlockData(GY62_UART);
    if(buffer[0] != 0x53) counter = 0;
    else
    {
        counter++;
        flags = 0x00;
    }

    if(counter == 10)
    {
        counter = 0;
        buffer[0] = 0x00;
        Roll = ((float) ((uint16_t) buffer[2] << 8 | (uint16_t) buffer[1])) / 32768.0f * 180.0f;
        Pitch = ((float) ((uint16_t) buffer[4] << 8 | (uint16_t) buffer[3])) / 32768.0f * 180.0f;
        Yaw = ((float) ((uint16_t) buffer[6] << 8 | (uint16_t) buffer[5])) / 32768.0f * 180.0f;
        flags = 0x01;
    }
}
