/*
 * lib_i2c.c
 *
 *  Created on: 2023/05/27
 *      Author: RATTAN
 */


#include "lib_i2c.h"

static uint16_t ticker;

/*
 * ����:I2C��ʼ��
 * I2CNUM   SCL     SDA
 *  0       PB2     PB3
 *  1       PA6     PA7
 *  2       PE4     PE5
 *  3       PD0     PD1
 * ����
 *  @I2CNum:
 *      I2C��:
 *          I2C0 - I2C3
 *  @SpeedMod:
 *      ����ģʽ:
 *          Speed_Mode_Normal ��׼ģʽ
 *          Speed_Mode_Fast ����ģʽ
 * ����ֵ:void
 * ��ע:None
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
 * ����:I2C���͵�����������Ĵ���
 * ����
 *  @I2CNum:
 *      I2C��:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      �����ַ
 *  @Reg_Addr:
 *      �Ĵ�����ַ
 *  @Data:
 *      ����
 * ����ֵ:void
 * ��ע:�����ַ��Ĭ������һλ����Ӷ�дλ.һ������������ַΪ���ƺ�ĵ�ַ,��OLEDģ��,������Ҫ���н�������һλ
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
 * ����:I2C����������
 * ����
 *  @I2CNum:
 *      I2C��:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      �����ַ
 *  @Reg_Addr:
 *      �Ĵ�����ַ
 *  @Dpoint:
 *      ָ���������������ͷָ��
 *  @counter:
 *      ��Ҫ���͵����ݵĴ�С,��λByte
 * ����ֵ:void
 * ��ע:�����ַ��Ĭ������һλ����Ӷ�дλ.һ������������ַΪ���ƺ�ĵ�ַ,��OLEDģ��,������Ҫ���н�������һλ
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
 * ����:I2C������Ĵ������յ��ֽ�����
 * ����
 *  @I2CNum:
 *      I2C��:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      �����ַ
 *  @Reg_Addr:
 *      �Ĵ�����ַ
 * ����ֵ:
 *      uint8_t:Ϊ���յ�������
 * ��ע:�����ַ��Ĭ������һλ����Ӷ�дλ.һ������������ַΪ���ƺ�ĵ�ַ,��OLEDģ��,������Ҫ���н�������һλ
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
 * ����:I2C��������
 * ����
 *  @I2CNum:
 *      I2C��:
 *          I2C0 - I2C3
 *  @Dev_Addr:
 *      �����ַ
 *  @Reg_Addr:
 *      �Ĵ�����ַ
 *  @Dpoint:
 *      ָ�����ݽ�����ͷ��ָ��
 *  @counter:
 *      ��Ҫ���յ����ݵĴ�С,��λByte
 * ����ֵ:void
 * ��ע:�����ַ��Ĭ������һλ����Ӷ�дλ.һ������������ַΪ���ƺ�ĵ�ַ,��OLEDģ��,������Ҫ���н�������һλ
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
