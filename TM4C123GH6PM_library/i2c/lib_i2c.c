/*
 * lib_i2c.c
 *
 *  Created on: 2023/05/27
 *      Author: RATTAN
 */


#include "lib_i2c.h"

static uint16_t ticker;

/*
 * 描述:I2C初始化
 * I2CNUM   SCL     SDA
 *  0       PB2     PB3
 *  1       PA6     PA7
 *  2       PE4     PE5
 *  3       PD0     PD1
 * 参数
 *  @I2CNum:
 *      I2C号:
 *          I2C0 - I2C3
 *  @SpeedMod:
 *      速率模式:
 *          Speed_Mode_Normal 标准模式
 *          Speed_Mode_Fast 快速模式
 * 返回值:void
 * 备注:None
 */
void LIB_I2C_Init(uint8_t I2CNum, uint8_t SpeedMod)
{
    uint32_t I2C_INIT_TABLE[4][5] = {{GPIO_PB2_I2C0SCL, GPIO_PB3_I2C0SDA, GPIO_PORTB_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOB},
                                     {GPIO_PA6_I2C1SCL, GPIO_PA7_I2C1SDA, GPIO_PORTA_BASE, GPIO_PIN_6, SYSCTL_PERIPH_GPIOA},
                                     {GPIO_PE4_I2C2SCL, GPIO_PE5_I2C2SDA, GPIO_PORTE_BASE, GPIO_PIN_4, SYSCTL_PERIPH_GPIOE},
                                     {GPIO_PD0_I2C3SCL, GPIO_PD1_I2C3SDA, GPIO_PORTD_BASE, GPIO_PIN_0, SYSCTL_PERIPH_GPIOD}};
    if(I2CNum < 4)
    {
        LIB_CLOCK_PeripheralClockEnable(I2C_INIT_TABLE[I2CNum][4]);
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_I2C0 + I2CNum);
        SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0 + I2CNum);
        GPIOPinConfigure(I2C_INIT_TABLE[I2CNum][0]);
        GPIOPinConfigure(I2C_INIT_TABLE[I2CNum][1]);
        GPIOPinTypeI2CSCL(I2C_INIT_TABLE[I2CNum][2], I2C_INIT_TABLE[I2CNum][3]);
        GPIOPinTypeI2C(I2C_INIT_TABLE[I2CNum][2], I2C_INIT_TABLE[I2CNum][3] << 1);
        I2CMasterInitExpClk(I2C0_BASE + (I2CNum << 12), SysCtlClockGet(), SpeedMod);
        HWREG((I2C0_BASE + (I2CNum << 12)) + I2C_O_FIFOCTL) = 80008000;
    }
}

/*
 * 描述:I2C发送单数据至外设寄存器
 * 参数
 *  @I2CNum:
 *      I2C号:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      外设地址
 *  @Reg_Addr:
 *      寄存器地址
 *  @Data:
 *      数据
 * 返回值:void
 * 备注:外设地址会默认左移一位后添加读写位.一般情况下外设地址为左移后的地址,如OLED模块,所以需要自行将其右移一位
 */

void LIB_I2C_Put(uint8_t I2CNum, uint16_t Dev_Addr, uint16_t Reg_Addr, uint8_t Data)
{
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, false);
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), Reg_Addr);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_START);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), Data);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_FINISH);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
}

/*
 * 描述:I2C发送数据流
 * 参数
 *  @I2CNum:
 *      I2C号:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      外设地址
 *  @Reg_Addr:
 *      寄存器地址
 *  @Dpoint:
 *      指向待发送数据区的头指针
 *  @counter:
 *      需要发送的数据的大小,单位Byte
 * 返回值:void
 * 备注:外设地址会默认左移一位后添加读写位.一般情况下外设地址为左移后的地址,如OLED模块,所以需要自行将其右移一位
 */
void LIB_I2C_Transmit(uint8_t I2CNum, uint16_t Dev_Addr, uint16_t Reg_Addr, const uint8_t* Dpoint, uint32_t counter)
{
    uint32_t count = 0;
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, false);
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), Reg_Addr);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_START);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    for(;count < counter - 1; count++)
    {
        I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), *(Dpoint + count));
        I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_CONT);
        ticker = timeout;
        while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    }
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), *(Dpoint + counter-1));
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_FINISH);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
}
/*
 * 描述:I2C从外设寄存器接收单字节数据
 * 参数
 *  @I2CNum:
 *      I2C号:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      外设地址
 *  @Reg_Addr:
 *      寄存器地址
 * 返回值:
 *      uint8_t:为接收到的数据
 * 备注:外设地址会默认左移一位后添加读写位.一般情况下外设地址为左移后的地址,如OLED模块,所以需要自行将其右移一位
 */
uint8_t LIB_I2C_Get(uint8_t I2CNum, uint16_t Dev_Addr, uint16_t Reg_Addr)
{
    uint8_t buffer = 0;
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, false);
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), Reg_Addr);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_START);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, true);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_SINGLE_RECEIVE);
    buffer = I2CMasterDataGet(I2C0_BASE + (I2CNum << 12));
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    return buffer;

}

/*
 * 描述:I2C接收数据
 * 参数
 *  @I2CNum:
 *      I2C号:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      外设地址
 *  @Reg_Addr:
 *      寄存器地址
 *  @Dpoint:
 *      指向数据接收区头的指针
 *  @counter:
 *      需要接收的数据的大小,单位Byte
 * 返回值:void
 * 备注:外设地址会默认左移一位后添加读写位.一般情况下外设地址为左移后的地址,如OLED模块,所以需要自行将其右移一位
 */
void LIB_I2C_Recive(uint8_t I2CNum, uint16_t Dev_Addr, uint16_t Reg_Addr, uint8_t* Dpoint, uint32_t counter)
{
    if(counter == 1)
    {
        *Dpoint = LIB_I2C_Get(I2CNum, Dev_Addr, Reg_Addr);
        return ;
    }
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, false);
    I2CMasterDataPut(I2C0_BASE + (I2CNum << 12), Reg_Addr);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_SEND_START);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    I2CMasterSlaveAddrSet(I2C0_BASE + (I2CNum << 12), Dev_Addr, true);
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_RECEIVE_START);
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    *(Dpoint + 0) = I2CMasterDataGet(I2C0_BASE + (I2CNum << 12));
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    for (uint32_t count = 1; count < counter - 1; ++count)
    {
        I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        *(Dpoint + count) = I2CMasterDataGet(I2C0_BASE + (I2CNum << 12));
        ticker = timeout;
        while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
    }
    I2CMasterControl(I2C0_BASE + (I2CNum << 12), I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    *(Dpoint + counter - 1) = I2CMasterDataGet(I2C0_BASE + (I2CNum << 12));
    ticker = timeout;
    while(I2CMasterBusy(I2C0_BASE + (I2CNum << 12)) && ticker --) delay_us(1);
}
