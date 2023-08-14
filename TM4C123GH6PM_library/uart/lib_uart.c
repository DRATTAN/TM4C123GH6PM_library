/*
 * lib_uart.c
 *
 *  Created on: 2023/05/21
 *      Author: RATTAN
 */

#include "lib_uart.h"

/*
 * 描述:初始化UARTx
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      波特率:
 *          典型值115200,9600,19200
 *  @UARTx_ISRhandle:
 *      串口接收回调函数:
 *          当不使用串口接收中断时,请仍创建一个函数填入,此函数中必须存在LIB_UART_Get(UARTx)或LIB_UART_GetBlock(UARTx),否则串口会溢出
 * 返回值:void
 * 备注:
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
 * 描述:初始化串口0为printf端口
 * 参数 void
 * 返回值:void
 * 备注:使用此函数后可以直接使用printf,无需其他操作.
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
 * 描述:串口发送一字节数据
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      待发送的数据
 * 返回值:void
 * 备注:
 */
void LIB_UART_SendData(uint8_t Uart_Ver, uint8_t Data)
{
    while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_TXFF) ;
    HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR) = Data;
}

/*
 * 描述:串口传输多位数据
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      待发送的数据存储区的地址
 *  @Counter:
 *      发送数据的大小,以字节为单位
 * 返回值:void
 * 备注:
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
 * 描述:串口发送字符串
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      待发送的字符串地址
 * 返回值:void
 * 备注:字符串必须以'\0'结尾,否则卡死.函数不会把'\0'一起发送出去
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
 * 描述:串口阻塞式接收一位数据
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 * 返回值:
 *      uint8_t
 *          接收到的字符
 * 备注:当接收区没有数据时,此函数会阻塞.直到接收到数据
 */
uint8_t LIB_UART_GetBlockData(uint8_t Uart_Ver)
{
    while(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_RXFE) ;
    return (uint8_t) HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR);
}
/*
 * 描述:串口非阻塞式接收一位数据
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      若接收到字符,接收到的字符将存储到此位置
 * 返回值:
 *      int8_t:
 *          若接收到字符,将返回0,若未接受到字符,将返回-1
 * 备注:当接收区没有数据时,此函数仅返回-1,不会对Buffer有任何操作
 */
int8_t LIB_UART_GetData(uint8_t Uart_Ver, uint8_t *Buffer)
{
    if(!(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_FR) & UART_FR_RXFE)) *Buffer = (uint8_t)(HWREG((UART_BASE + (Uart_Ver << 12)) + UART_O_DR));
    else return(-1);
    return 0;
}

/*
 * 描述:设置串口中断服务函数
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 *      中服务函数
 * 返回值:void
 * 备注:
 */
void LIB_UART_UartIsrSet(uint8_t Uart_Ver, void (*UARTx_ISRhandle)(void))
{
    LIB_ISR_UARTRegister(Uart_Ver, UARTx_ISRhandle);
}

/*
 * 描述:失能串口接收中断函数
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 * 返回值:void
 * 备注:
 */
void LIB_UART_IntDisable(uint8_t Uart_Ver)
{
    UARTIntDisable((UART_BASE + (Uart_Ver << 12)), UART_INT_RX);
}

/*
 * 描述:使能串口接收中断函数
 * 参数
 *  @Uart_Ver:
 *      串口号:
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
 * 返回值:void
 * 备注:
 */
void LIB_UART_IntEnable(uint8_t Uart_Ver)
{
    UARTIntEnable((UART_BASE + (Uart_Ver << 12)), UART_INT_RX);
}
