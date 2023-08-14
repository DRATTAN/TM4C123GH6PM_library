/*
 * oled.c
 *
 *  Created on: 2023/03/05
 *      Author: RATTAN
 */



#include "oled.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/* OLED I2C 接口*/
static void Oled_Cmd(uint8_t cmd)
{
    LIB_I2C_Put(I2C_NUM, Oled_Addr, 0x00, cmd);
}

static void Oled_Datas(const uint8_t* pdata, uint32_t counter)
{
    LIB_I2C_Transmit(I2C_NUM, Oled_Addr, 0x40, pdata, counter);
}

/*
 * 描述:OLED 初始化
 * 参数:
 *      void
 * 返回值:void
 * 备注:请在初始化 OLED 前先初始化I2C
 */
void Oled_Init()
{
    delay_ms(100);

        Oled_Cmd(0xAE); // 关闭OLED -- turn off oled panel

       Oled_Cmd(0xD5); // 设置显示时钟分频因子/振荡器频率 -- set display clock divide ratio/oscillator frequency
       Oled_Cmd(0x80); // A[3:0] 分频因子, A[7:4] 震荡频率设置 Set Clock as 100 Frames/Sec

       Oled_Cmd(0x20); // 设置内存寻址模式 -- Set Memory Addressing Mode (0x00 / 0x01 / 0x02)
       Oled_Cmd(0x02); // \ Page Addressing 设置为页寻址模式

       Oled_Cmd(0xA8); // 设置多路传输比率 -- set multiplex ratio (16 to 63)
       Oled_Cmd(0x3F); // \ 1 / 64 duty

       Oled_Cmd(0xDA); // 设置列引脚硬件配置 -- set com pins hardware configuration
       Oled_Cmd(0x12); // \ Sequential COM pin configuration，Enable COM Left/Right remap

       /* ----- 方向显示配置 ----- */
       Oled_Cmd(0xA1); // 设置段重映射 -- Set SEG / Column Mapping     0xA0左右反置（复位值） 0xA1正常（重映射值）
       Oled_Cmd(0xC8); // 设置行输出扫描方向 -- Set COM / Row Scan Direction   0xc0上下反置（复位值） 0xC8正常（重映射值）
       /* ----- END ----- */

       Oled_Cmd(0x40); // 设置设置屏幕（GDDRAM）起始行 -- Set Display Start Line (0x40~0x7F)

       Oled_Cmd(0xD3); // 设置显示偏移 -- set display offset (0x00~0x3F)
       Oled_Cmd(0x00); // \ not offset 偏移值是 0

       Oled_Cmd(0x81); // 设置对比度 -- set contrast control register (0x00~0x100)
       Oled_Cmd(0xCF); // \ Set SEG Output Current Brightness

       Oled_Cmd(0xD9); // 设置预充电期间的持续时间 -- set pre-charge period
       Oled_Cmd(0xF1); // \ Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

       Oled_Cmd(0xDB); // 调整VCOMH调节器的输出 -- set vcomh (0x00 / 0x20 / 0x30)
       Oled_Cmd(0x20); // \ Set VCOM Deselect Level

       Oled_Cmd(0x8D); // 电荷泵设置 -- set Charge Pump enable / disable (0x14 / 0x10)
       Oled_Cmd(0x14); // \ 启用电荷泵 Enable charge pump during display on

       Oled_Cmd(0xA4); //  全局显示开启(黑屏/亮屏)  (0xA4 根据RAM 内容显示 / 0xA5 每个oled都点亮 全部显示)

       Oled_Cmd(0xA6); // 设置显示方式(正常/反显) -- set normal display (0xA6 / 0xA7)

       Oled_Cmd(0xAF); // 打开OLED -- turn on oled panel ： OXAE 关闭OLED 休眠模式 0xaf 打开oled
       Oled_Clear();
}

/*
 * 描述:OLED 清屏
 * 参数:
 *      void
 * 返回值:void
 * 备注:None
 */
void Oled_Clear()
{
    Oled_Fill(0x00);
}

/*
 * 描述:OLED 清行
 * 参数
 *  @x:
 *      行号 0 - 7
 *  @y:
 *      列号 0 - 127
 *  @length:
 *      需要清理的长度,不可超过行尾
 * 备注:None
 */
void Oled_Clear_Part(uint8_t x, uint8_t y, uint8_t length)
{
    uint8_t i = 0;
    Oled_Cmd(0xb0+y);
    Oled_Cmd(((x&0xf0)>>4)|0x10);
    Oled_Cmd((x&0x0f)|0x01);
    for (i = 0; i < length; i++) Oled_Datas((uint8_t*)F6x8[0], 6);
}

/*
 * 描述:OLED 填充
 * 参数
 *  @Data:
 *      填充的数据
 * 备注:None
 */
void Oled_Fill(uint8_t Data)
{
    uint8_t m;
    uint8_t dis_buffer[128];
    for(m=0;m<128;m++) dis_buffer[m]=Data;
    for(m=0;m<8;m++)
    {
        Oled_Cmd(0xb0+m);
        Oled_Datas(dis_buffer, 128);
    }
}

/*
 * 描述:OLED 设置光标位置
 * 参数
 *  @x:
 *      行号 0 - 7
 *  @y:
 *      列号 0 - 127
 * 备注:None
 */
void Oled_SetPos(uint8_t x, uint8_t y)
{
    Oled_Cmd(0xb0+y);
    Oled_Cmd(((x&0xf0)>>4)|0x10);
    Oled_Cmd((x&0x0f)|0x01);
}

/*
 * 描述:OLED 显示字符
 * 参数
 *  @x:
 *      行号 0 - 7
 *  @y:
 *      列号 0 - 127
 *  @c:
 *      需要显示的字符
 * 备注:None
 */
void Oled_ShowChar(uint8_t x, uint8_t y, char c)
{
    if(x>127 || y>7) return;
    Oled_SetPos(x,y);
    Oled_Datas((uint8_t*)F6x8[(uint8_t)(c-32)], 6);
}

/*
 * 描述:OLED 显示字符串
 * 参数
 *  @x:
 *      行号 0 - 7
 *  @y:
 *      列号 0 - 127
 *  @String_Addr:
 *      需要显示的字符串的首地址
 * 备注:None
 */
void Oled_ShowString(uint8_t x, uint8_t y, const char* String_Addr)
{
    uint8_t lenth=strlen(String_Addr),i=0;
    int16_t j=x;
    Oled_SetPos(x,y);
    for(i=0;i<lenth;i++)
    {
        j+=6;
        if(((int16_t)127-j<=(int16_t)0) || (*(String_Addr+i) == 0x00))
        {
            return;
        }
        Oled_Datas((uint8_t*)F6x8[(uint8_t)(*(String_Addr+i)-32)], 6);
    }
}


/// @brief 显示32位无符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowUint32(uint8_t x, uint8_t y, const uint32_t Data)
{
    char buffer[11];
    sprintf(buffer, "%-10u", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示32位有符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowInt32(uint8_t x,uint8_t y,const int32_t Data)
{
    char buffer[12];
    sprintf(buffer, "%-11d", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示16位无符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowUint16(uint8_t x,uint8_t y,const uint16_t Data)
{
    char buffer[6];
    sprintf(buffer, "%-5u", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示16位有符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowInt16(uint8_t x,uint8_t y,const int16_t Data)
{
    char buffer[7];
    sprintf(buffer, "%-6d", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示8位无符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowUint8(uint8_t x,uint8_t y,const uint8_t Data)
{
    char buffer[4];
    sprintf(buffer, "%-3u", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示8位有符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowInt8(uint8_t x,uint8_t y,const int8_t Data)
{
    char buffer[5];
    sprintf(buffer, "%-4d", Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 显示浮点数(默认保留小数点后2位)
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param Data 待显示数字
void Oled_ShowFloat(uint8_t x,uint8_t y,const float Data)
{
    char buffer[15];
    sprintf(buffer, "%-6.2f",Data);
    Oled_ShowString(x, y, buffer);
}

/// @brief 带参数名显示8位无符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param param_name 参数名
/// @param param_value 参数值
void Oled_ShowParamUint8(uint8_t x, uint8_t y, const char* param_name, const uint8_t param_value)
{
   Oled_ShowString(x, y, param_name);
   Oled_ShowUint8(x + strlen(param_name)*6, y, param_value);
}

/// @brief 带参数名显示8位有符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param param_name 参数名
/// @param param_value 参数值
void Oled_ShowParamInt8(uint8_t x, uint8_t y, const char* param_name, const int8_t param_value)
{
    Oled_ShowString(x, y, param_name);
    Oled_ShowInt8(x + strlen(param_name)*6, y, param_value);
}

/// @brief 带参数名显示16位无符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param param_name 参数名
/// @param param_value 参数值
void Oled_ShowParamUint16(uint8_t x, uint8_t y, const char* param_name, const uint16_t param_value)
{
    Oled_ShowString(x, y, param_name);
    Oled_ShowUint16(x + strlen(param_name)*6, y, param_value);
}

/// @brief 带参数名显示16位有符号整形数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param param_name 参数名
/// @param param_value 参数值
void Oled_ShowParamInt16(uint8_t x, uint8_t y, const char* param_name, const int16_t param_value)
{
    Oled_ShowString(x, y, param_name);
    Oled_ShowInt16(x + strlen(param_name)*6, y, param_value);
}
/// @brief 带参数名显示浮点数
/// @param x 数字横坐标
/// @param y 数字纵坐标
/// @param param_name 参数名
/// @param param_value 参数值
void Oled_ShowParamFloat(uint8_t x, uint8_t y, const char* param_name, const float param_value)
{
    Oled_ShowString(x, y, param_name);
    Oled_ShowFloat(x + strlen(param_name)*6, y, param_value);
}

/// @brief 显示bmp图片
/// @param x 图片左上角横坐标
/// @param y 图片左上角纵坐标
/// @param weight 图片宽度
/// @param high 图片高度
/// @param Bmp_Addr 图片数据头地址
void Oled_ShowBmp(uint8_t x, uint8_t y, uint8_t weight, uint8_t high, uint8_t* Bmp_Addr)
{

    __nop();
}

void Oled_ShowLine(uint8_t y, const uint8_t* Line_Addr)
{
    Oled_SetPos(0, y);
    Oled_Datas(Line_Addr, 128);
}

void Oled_test(uint8_t x, uint8_t y, const int16_t param_value)
{
    char buffer[7];
    buffer[6] = '\0';
    sprintf(buffer,"%d",param_value);
    Oled_ShowString(x, y, buffer);
}
