/*
 * lib_uart.c
 *
 *  Created on: 2023/05/21
 *      Author: RATTAN
 */

#include "lib_uart.h"

/*
 * ����:��ʼ��UARTx
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1     /VPC
 *          1       PB0     PB1     /BLU
 *          2       PD6     PD7     /QEI0
 *          3       PC6     PC7     /QEI1
 *          4       PC4     PC5     /QEI1
 *          5       PE4     PE5     /S
 *          6       PD4     PD5     /NON
 *          7       PE0     PE1     /MV
 *  @Baud:
 *      ������:
 *          ����ֵ115200,9600,19200
 *  @UARTx_ISRhandle:
 *      ���ڽ��ջص�����:
 *          ����ʹ�ô��ڽ����ж�ʱ,���Դ���һ����������,�˺����б������LIB_UART_Get(UARTx)��LIB_UART_GetBlock(UARTx),���򴮿ڻ����
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_Init(uint8_t Uart_Ver, uint32_t Baud, void (*UARTx_ISRhandle)(void))
{
    uint32_t INT_UART_TABLE[8] = {INT_UART0, INT_UART1, INT_UART2, INT_UART3, INT_UART4, INT_UART5, INT_UART6, INT_UART7};

    switch(Uart_Ver)
    {
    case UART0:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        break;
    case UART1:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinConfigure(GPIO_PB0_U1RX);
        GPIOPinConfigure(GPIO_PB1_U1TX);
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        break;
    case UART2:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOD);
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
        GPIOPinConfigure(GPIO_PD6_U2RX);
        GPIOPinConfigure(GPIO_PD7_U2TX);
        GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        break;
    case UART3:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOC);
        GPIOPinConfigure(GPIO_PC6_U3RX);
        GPIOPinConfigure(GPIO_PC7_U3TX);
        GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
        break;
    case UART4:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOC);
        GPIOPinConfigure(GPIO_PC4_U4RX);
        GPIOPinConfigure(GPIO_PC5_U4TX);
        GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
        break;
    case UART5:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinConfigure(GPIO_PE4_U5RX);
        GPIOPinConfigure(GPIO_PE5_U5TX);
        GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
        break;
    case UART6:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinConfigure(GPIO_PD4_U6RX);
        GPIOPinConfigure(GPIO_PD5_U6TX);
        GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
        break;
    case UART7:
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinConfigure(GPIO_PE0_U7RX);
        GPIOPinConfigure(GPIO_PE1_U7TX);
        GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        break;
    }
    LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_UARTBASE + Uart_Ver);

    LIB_ISR_UARTRegister(Uart_Ver, UARTx_ISRhandle);
    UARTConfigSetExpClk((UART_BASE + (Uart_Ver << 12)), SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    IntEnable(INT_UART_TABLE[Uart_Ver]);
    UARTIntEnable((UART_BASE + (Uart_Ver << 12)), UART_INT_RX);
    UARTFIFODisable((UART_BASE + (Uart_Ver << 12)));
    return;
}

/*
 * ����:��ʼ������0Ϊprintf�˿�
 * ���� void
 * ����ֵ:void
 * ��ע:ʹ�ô˺��������ֱ��ʹ��printf,������������.
 */
void LIB_UART_PrintfInit(uint8_t UART_NUM)
{
    if(UART_NUM == 0)
    {
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_UART0);
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
        UARTStdioConfig(0,115200, 16000000);
        IntEnable(INT_UART0);
        UARTIntEnable(UART0_BASE, UART_INT_RX);
        //LIB_ISR_UARTRegister(Uart_Ver, UARTx_ISRhandle);
    }
    else if(UART_NUM == 1)
    {
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_UART1);
        LIB_CLOCK_PeripheralClockEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinConfigure(GPIO_PB0_U1RX);
        GPIOPinConfigure(GPIO_PB1_U1TX);
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
        UARTStdioConfig(0,115200, 16000000);
        IntEnable(INT_UART1);
        UARTIntEnable(UART1_BASE, UART_INT_RX);
        //LIB_ISR_UARTRegister(Uart_Ver, UARTx_ISRhandle);
    }
}

/*
 * ����:���ڷ���һ�ֽ�����
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *  @Data:
 *      �����͵�����
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_SendData(uint8_t Uart_Ver, uint8_t Data)
{
    while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_TXFF) ;
    HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR) = Data;
}

/*
 * ����:���ڴ����λ����
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *  @*Buffer:
 *      �����͵����ݴ洢���ĵ�ַ
 *  @Counter:
 *      �������ݵĴ�С,���ֽ�Ϊ��λ
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_TransmitData(uint8_t Uart_Ver, uint8_t *Buffer,  uint32_t Counter)
{
    while(Counter--)
    {
        while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_TXFF) ;
        HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR) = *Buffer;
        Buffer ++;
    }
}

/*
 * ����:���ڷ����ַ���
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *  @*Str:
 *      �����͵��ַ�����ַ
 * ����ֵ:void
 * ��ע:�ַ���������'\0'��β,������.���������'\0'һ���ͳ�ȥ
 */
void LIB_UART_SendString(uint8_t Uart_Ver, uint8_t *Str)
{
    while(*Str != '\0')
    {
        while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_TXFF) ;
        HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR) = *Str;
        Str ++;
    }
}

/*
 * ����:��������ʽ����һλ����
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *
 * ����ֵ:
 *      uint8_t
 *          ���յ����ַ�
 * ��ע:��������û������ʱ,�˺���������.ֱ�����յ�����
 */
uint8_t LIB_UART_GetBlockData(uint8_t Uart_Ver)
{
    while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_RXFE) ;
    return (uint8_t) HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR);
}
/*
 * ����:���ڷ�����ʽ����һλ����
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *  @*Buffer:
 *      �����յ��ַ�,���յ����ַ����洢����λ��
 * ����ֵ:
 *      int8_t:
 *          �����յ��ַ�,������0,��δ���ܵ��ַ�,������-1
 * ��ע:��������û������ʱ,�˺���������-1,�����Buffer���κβ���
 */
int8_t LIB_UART_GetData(uint8_t Uart_Ver, uint8_t *Buffer)
{
    if(!(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_RXFE)) *Buffer = (uint8_t)(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR));
    else return(-1);
    return 0;
}

/*
 * ����:���ô����жϷ�����
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 *  @*UARTx_ISRhandle:
 *      �з�����
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_UartIsrSet(uint8_t Uart_Ver, void (*UARTx_ISRhandle)(void))
{
    LIB_ISR_UARTRegister(Uart_Ver, UARTx_ISRhandle);
}

/*
 * ����:ʧ�ܴ��ڽ����жϺ���
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_IntDisable(uint8_t Uart_Ver)
{
    UARTIntDisable((UART_BASE + (Uart_Ver << 12)), UART_INT_RX);
}

/*
 * ����:ʹ�ܴ��ڽ����жϺ���
 * ����
 *  @Uart_Ver:
 *      ���ں�:
 *          UART0-UART7
 *          uart    rx      tx
 *          0       PA0     PA1
 *          1       PB0     PB1
 *          2       PD6     PD7
 *          3       PC6     PC7
 *          4       PC4     PC5
 *          5       PE4     PE5
 *          6       PD4     PD5
 *          7       PE0     PE1
 * ����ֵ:void
 * ��ע:
 */
void LIB_UART_IntEnable(uint8_t Uart_Ver)
{
    UARTIntEnable((UART_BASE + (Uart_Ver << 12)), UART_INT_RX);
}
