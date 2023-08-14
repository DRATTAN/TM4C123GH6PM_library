/*
 * tsl1401.c
 *
 *  Created on: 2023/07/25
 *      Author: RATTAN
 */

#include "tsl1401.h"

static uint16_t vol[8];
void Tsl1401_Init()
{
    LIB_GPIO_Init(TSL1401_CLK_PORT, TSL1401_CLK_PIN, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_RESET);
    LIB_GPIO_Init(TSL1401_SI_PORT, TSL1401_SI_PIN, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_RESET);
}

void Tsl1401_GetData(uint16_t exp_time,uint8_t *buffer)
{
    CLK_WRITE(1);
    SI_WRITE(0);
    delay_us(1);
    SI_WRITE(1);
    CLK_WRITE(0);
    delay_us(1);

    CLK_WRITE(1);
    SI_WRITE(0);

    delay_us(1);
    for(uint8_t i = 0;i<128;i++)
    {
        CLK_WRITE(0);
        delay_systick(exp_time);
        LIB_ADC_GetValue(vol);
        buffer[i] = vol[TSL1401_A0_CHANNEL] >> 4;
        CLK_WRITE(1);
        delay_us(1);
    }
}
void Ts1401_Trans(uint8_t* dpoint)
{
    int16_t buffer[128];
    int16_t maxdef = 0, mindef = 0;
    uint8_t risingsub = 0, fallingsub = 0;
    uint8_t leftsub = 63, rightsub = 65;
    uint8_t risingsublast = 0, fallingsublast = 0;
    for(uint8_t i = 0; i < 128; i++) buffer[i] = 0;
    for(uint8_t i = 2; i < 126; i++)
    {
        buffer[i] = (int16_t)dpoint[i] - (int16_t)dpoint[i - 2];
    }
    for(uint8_t i = 1; i < 127; i++)
    {
        if(buffer[i] > maxdef)
        {
           maxdef = buffer[i];
           risingsub = i;
        }
        if(buffer[i] < mindef)
        {
            mindef = buffer[i];
            fallingsub = i;
        }
    }

    if(risingsub <= fallingsub)
    {
        risingsub = risingsublast;
        fallingsub  = fallingsublast;
    }
    else
    {
        risingsublast = risingsub;
        fallingsublast  = fallingsub;

    }
    if(risingsub > fallingsub)
    {
        leftsub = fallingsub;
        rightsub = risingsub;
    }
    else
    {
        leftsub = risingsub;
        rightsub = fallingsub;
    }
    memset(dpoint, 0xff, 128);
    memset(dpoint + leftsub, 0x00, rightsub - leftsub + 1);

//    static uint16_t Left, Right, CCD_Zhongzhi;
//    static uint16_t value1_max, value1_min;
//    static uint16_t CCD_Yuzhi;
//    value1_max = dpoint[0];
//    for(uint8_t i = 5; i < 123; i++)
//    {
//        if(value1_max <= dpoint[i])
//        value1_max = dpoint[i];
//    }
//    value1_min = dpoint[0];
//    for(uint8_t i = 5;i < 123; i++)
//    {
//        if(value1_min >= dpoint[i])
//        value1_min = dpoint[i];
//    }
//    CCD_Yuzhi = (value1_max + value1_min) / 2;
//    for(uint8_t i = 5; i < 118; i++)
//    {
//        if(dpoint[i] > CCD_Yuzhi && dpoint[i + 1] > CCD_Yuzhi && dpoint[i + 2] > CCD_Yuzhi && dpoint[i + 3] < CCD_Yuzhi && dpoint[i + 4] < CCD_Yuzhi && dpoint[i + 5] < CCD_Yuzhi)
//        {
//            Left = i;
//            break;
//        }
//    }
//    for(uint8_t j = 118; j>5; j--)
//    {
//        if(dpoint[j] < CCD_Yuzhi && dpoint[j + 1] < CCD_Yuzhi && dpoint[j + 2] < CCD_Yuzhi && dpoint[j + 3] > CCD_Yuzhi && dpoint[j + 4] > CCD_Yuzhi && dpoint[j + 5] > CCD_Yuzhi)
//        {
//            Right = j;
//            break;
//        }
//    }
//    CCD_Zhongzhi = (Right + Left) / 2;
//    memset(dpoint, 0xFF, 128);
//    memset(dpoint + CCD_Zhongzhi - 2, 0x00, 5);
}

//uint8_t Ts1401_GetThreshold(const uint8_t *buffer)
//{
//    uint8_t nList[256] = { 0 };
//    uint8_t threshold = 0, num = 0, cnt = 0;
//    int32_t avfor = 0, avback = 0;
//    uint32_t cnts = 0;
//    uint32_t total = 0, total_low = 0, total_high = 0, g = 0, max = 0;
//
//    memset(nList, 0x00, 256);
//
//    for(uint8_t counter = 0; counter < 128; counter ++)
//    {
//        num = *(buffer + counter);
//        nList[num] += 1;
//        total += num;
//    }
//
//    for(uint16_t counter = 0; counter < 256; counter ++)
//    {
//        cnt = nList[counter];
//        if(cnt == 0) continue;
//        total_low += cnt * counter;
//        cnts += cnt;
//        if(cnts == 128) break;
//        total_high = total - total_low;
//        avfor = (int32_t)(total_low / cnts);
//        avback = (int32_t)(total_high / (128 - cnts));
//        g=(avback-avfor) * (avback-avfor) * (cnts * 128 - cnts * cnts) / (128 * 128);
//        if(max < g)
//        {
//            max = g;
//            threshold = counter;            //方差最大的灰度值即为大津阈值
//        }
//    }
//
////    uint16_t threshold = 0;
////    uint8_t max = 0, min = 255;
////    for(uint8_t counter = 0; counter < 128; counter++)
////    {
////        if(buffer[counter] < min) min = buffer[counter];
////        if(buffer[counter] > max) max = buffer[counter];
////    }
////    threshold = ((uint16_t) max + (uint16_t) min) / 2;
//    return threshold;
//}

void Tsl1401_Show(uint8_t y, uint8_t is_visible,const uint8_t *buffer)
{
    if(is_visible == 0) Oled_ShowLine(y, buffer);
    else
    {
        uint8_t temp[128];
        for(uint8_t i = 0; i < 128; i++)
        {
            temp[i] = 0xFF << 8 - (uint8_t)(((uint16_t)buffer[i] + 1) / 32);
        }
        Oled_ShowLine(y, temp);
    }
}


