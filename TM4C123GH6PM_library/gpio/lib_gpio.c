/*EK-TM4C123GXL_LIB
 * 作者:RATTAN
 * 文件描述:none
 */

#include "lib_gpio.h"


void LIB_GPIO_UnlockAllPins()
{
    GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOUnlockPin(GPIO_PORTD_BASE, GPIO_PIN_7);
}


/*
 * 描述:初始化GPIO端口
 * 参数
 *  @Port:
 *      GPIO基地址:
 *          GPIOA-GPIOF
 *  @Pin:
 *      端口号:
 *          GPIO_PIN_0-GPIO_PIN_7
 *  @Dir:
 *      方向:
 *          GPIO_DIR_OUTPUT     输出,
 *          GPIO_DIR_INPUT      输入
 *  @Type:
 *      输 入/出 配置,当Pin配置为输入时,可以配置为:
 *          GPIO_PIN_TYPE_INPUT_WPU     弱上拉
 *          GPIO_PIN_TYPE_INPUT_WPD     弱下拉
 *      当配置Pin为输出时,可以配置为:
 *          GPIO_PIN_TYPE_OUTPUT_STD    推挽输出
 *          GPIO_PIN_TYPE_OUTPUT_OD     开漏输出
 *  @Value:
 *      默认输出电平,仅 Dir 为 GPIO_DIR_OUTPUT 时有效,可以配置为:
 *          GPIO_PIN_SET        高电平
 *          GPIO_PIN_RESET      低电平
 *      当 Dir 为 GPIO_DIR_OUTPUT 时,此参数会被忽略,建议填入0x00
 *
 * 返回值:void
 * 备注:可以同时初始化多个相同GPIOx下的端口,例如
 * LIB_GPIO_Init(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD, GPIO_PIN_SET);
 */
void LIB_GPIO_Init(uint32_t Port, uint8_t Pin, uint8_t Dir, uint32_t Type, uint8_t Value)
{
    if(Port >= GPIOE) LIB_CLOCK_PeripheralClockEnable(((Port - GPIOE)>>12) + SYSCTL_PERIPH_GPIOE);
    else if(Port >= GPIOA) LIB_CLOCK_PeripheralClockEnable(((Port - GPIOA)>>12) + SYSCTL_PERIPH_GPIOA);
    else __nop();
    if(Port == GPIOF && (Pin & GPIO_PIN_0)== GPIO_PIN_0)
    {
        GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);

    }
    if(Port == GPIOD && Pin == (Pin & GPIO_PIN_7)== GPIO_PIN_7)
    {
        GPIOUnlockPin(GPIO_PORTD_BASE, GPIO_PIN_7);
    }
    if(GPIO_DIR_OUTPUT == Dir)
    {
        GPIOPadConfigSet(Port, Pin, GPIO_STRENGTH_2MA, Type);
        HWREG(Port + GPIO_O_DIR) = ((GPIO_DIR_MODE_OUT & 1) ? (HWREG(Port + GPIO_O_DIR) | Pin) : (HWREG(Port + GPIO_O_DIR) & ~(Pin)));
        HWREG(GPIO_DIR_MODE_OUT + GPIO_O_AFSEL) = ((GPIO_DIR_MODE_OUT & 2) ? (HWREG(Port + GPIO_O_AFSEL) | Pin) : (HWREG(Port + GPIO_O_AFSEL) & ~(Pin)));
        LIB_GPIO_WritePin(Port, Pin, Value);
    }

    else if(GPIO_DIR_INPUT == Dir)
    {
        HWREG(GPIO_DIR_MODE_IN + GPIO_O_DIR) = ((GPIO_DIR_MODE_IN & 1) ? (HWREG(Port + GPIO_O_DIR) | Pin) : (HWREG(Port + GPIO_O_DIR) & ~(Pin)));
        HWREG(GPIO_DIR_MODE_IN + GPIO_O_AFSEL) = ((GPIO_DIR_MODE_IN & 2) ? (HWREG(Port + GPIO_O_AFSEL) | Pin) : (HWREG(Port + GPIO_O_AFSEL) & ~(Pin)));
        GPIOPadConfigSet(Port, Pin, GPIO_STRENGTH_2MA, Type);
    }
    else return ;
}



/*
 * 描述:对GPIO的PIN写入数据(设置电平)
 * 参数
 *  @Port:
 *      GPIO基地址:
 *          GPIOA-GPIOF
 *  @Pin:
 *      端口号:
 *          GPIO_PIN_0-GPIO_PIN_7
 *  @Value:
 *      设置的电平,可以配置为:
 *      GPIO_PIN_SET        输出高电平
 *      GPIO_SET_RESET      输出低电平
 * 返回值:void
 * 备注:可以同时输出相同GPIOx多个端口下的Pin于相同电压,例如LIB_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
 */
void LIB_GPIO_WritePin(uint32_t Port, uint32_t Pin, uint8_t Value)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + (Pin << 2)));
    if(GPIO_PIN_RESET == Value) HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg & ~((uint8_t)Pin));
    else if(GPIO_PIN_SET == Value) HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg | (uint8_t)Pin);
    else return;
}


/*
 * 描述:读取GPIO端口的Pin的电平
 * 参数
 *  @Port:
 *      GPIO基地址:
 *          GPIOA-GPIOF
 *  @Pin:
 *      端口号:
 *          GPIO_PIN_0-GPIO_PIN_7
 * 返回值:uint8_t
 *          返回值为读取到的电平的值.
 * 备注:可以同时读取相同GPIOx下的多个Pin的值,例如LIB_GPIO_ReadPin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);当这些Pin中有任何
 * 一个为GPIO_PIN_SET时则返回GPIO_PIN_SET,否则返回GPIO_PIN_RESET.建议一次只读取一个Pin
 */
uint8_t LIB_GPIO_ReadPin(uint32_t Port, uint32_t Pin)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + (Pin << 2))) & (uint8_t)Pin;
    if(reg) return GPIO_PIN_SET;
    else return GPIO_PIN_RESET;
}

/*
 * 描述:翻转GPIO端口Pin的电平
 * 参数
 *  @Port:
 *      GPIO基地址:
 *          GPIOA-GPIOF
 *  @Pin:
 *      端口号:
 *          GPIO_PIN_0
 * 返回值:void
 * 备注:可以同时翻转GPIOx下的多个Pin的电平,例如LIB_GPIO_TogglePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);每个Pin的的电平会被分别取反并写入
 */
void LIB_GPIO_TogglePin(uint32_t Port, uint32_t Pin)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + ((uint8_t)Pin << 2)));
    HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg ^ (uint8_t)Pin);
}


/*
 * 描述:初始化GPIOx下单个Pinx的中断
 * 参数
 *      @GPIOISRPort:
 *          GPIO中断的端口号:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO中断的端口Pin号:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @Trigger:
 *          触发方式，可选的参数有:
 *               GPIO_EXTI_TRIGGER_RISING 上升沿触发
 *               GPIO_EXTI_TRIGGER_FALLING 下降沿触发
 *               GPIO_EXTI_TRIGGER_BOTHEDGES 双边沿触发
 *               GPIO_EXTI_TRIGGER_HIGH 高电平触发
 *               GPIO_EXTI_TRIGGER_LOW 低电平触发
 *               GPIO_EXTI_TRIGGER_DISCRETE_INT 离散触发
 *      @ISR_handler:
 *          中断回调函数，不允许存在返回值，不允许传入参数.
 * 返回值:void
 * 备注:(2023/05/14) @param(Trigger) GPIO_EXTI_TRIGGER_DISCRETE_INT未实现,请勿使用.
 *
 */
void LIB_GPIO_ExtiInit(uint8_t ISRGPIOPort, uint8_t ISRGPIOPin, uint8_t Trigger, void (*ISR_handler)(void))
{
    uint32_t GPIO_BASE_TABLE[6] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
    uint32_t GPIO_EXTI_TRIG_TABLE[6] = {GPIO_RISING_EDGE, GPIO_FALLING_EDGE, GPIO_BOTH_EDGES, GPIO_HIGH_LEVEL, GPIO_LOW_LEVEL, GPIO_DISCRETE_INT};
    uint32_t GPIO_ASSIGNMENTS_TABLE[6] = {INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF};
    if(Trigger == GPIO_EXTI_TRIGGER_RISING || Trigger == GPIO_EXTI_TRIGGER_HIGH) LIB_GPIO_Init(GPIO_BASE_TABLE[ISRGPIOPort], 0x01 << ISRGPIOPin, GPIO_DIR_INPUT, GPIO_PIN_TYPE_INPUT_WPD, 0x00);
    else if(Trigger == GPIO_EXTI_TRIGGER_FALLING || Trigger == GPIO_EXTI_TRIGGER_LOW || Trigger == GPIO_EXTI_TRIGGER_BOTHEDGES) LIB_GPIO_Init(GPIO_BASE_TABLE[ISRGPIOPort], 0x01 << ISRGPIOPin, GPIO_DIR_INPUT, GPIO_PIN_TYPE_INPUT_WPU, 0x00);
    GPIOIntTypeSet(GPIO_BASE_TABLE[ISRGPIOPort], 0x01 << ISRGPIOPin, GPIO_EXTI_TRIG_TABLE[Trigger]);
    LIB_ISR_GPIOEXTIRegister(ISRGPIOPort,ISRGPIOPin,ISR_handler);
    GPIOIntEnable(GPIO_BASE_TABLE[ISRGPIOPort], 0x01 << ISRGPIOPin);
    IntEnable(GPIO_ASSIGNMENTS_TABLE[ISRGPIOPort]);
}

/*
 * 描述:打开/关闭GPIOx 下某个Pin的外部中断
 * 参数
 *      @GPIOISRPort:
 *          GPIO中断的端口号:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO中断的端口Pin号:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @Ctrl:
 *          打开/关闭:
 *               GPIO_EXTI_CTRL_ENABLE 打开
 *               GPIO_EXTI_CTRL_DISABLE 关闭
 *      @ISR_handler:
 *          中断回调函数，不允许存在返回值，不允许传入参数.
 * 返回值:void
 * 备注:
 */
void LIB_GPIO_ExtiCtrlPin(uint8_t GPIOISRPort, uint8_t GPIOISRPin, uint8_t Ctrl)
{
    uint32_t GPIO_BASE_TABLE[6] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
    if(GPIO_EXTI_CTRL_ENABLE == Ctrl) GPIOIntEnable(GPIO_BASE_TABLE[GPIOISRPort], 0x01 << GPIOISRPin);
    if(GPIO_EXTI_CTRL_DISABLE == Ctrl) GPIOIntDisable(GPIO_BASE_TABLE[GPIOISRPort], 0x01 << GPIOISRPin);
}

/*
 * 描述:打开/关闭GPIOx的外部中断
 * 参数
 *      @GPIOISRPort:
 *          GPIO中断的端口号:
 *              ISR_GPIOA-ISR_GPIOF
 *      @Ctrl:
 *          打开/关闭:
 *               GPIO_EXTI_CTRL_ENABLE 打开
 *               GPIO_EXTI_CTRL_DISABLE 关闭
 *      @ISR_handler:
 *          中断回调函数，不允许存在返回值，不允许传入参数.
 * 返回值:void
 * 备注:
 */
void LIB_GPIO_ExtiCtrlPort(uint8_t GPIOISRPort, uint8_t Ctrl)
{
    uint32_t GPIO_ASSIGNMENTS_TABLE[6] = {INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF};
    if(GPIO_EXTI_CTRL_ENABLE == Ctrl) IntEnable(GPIO_ASSIGNMENTS_TABLE[GPIOISRPort]);
    if(GPIO_EXTI_CTRL_DISABLE == Ctrl) IntDisable(GPIO_ASSIGNMENTS_TABLE[GPIOISRPort]);
}

/*
 * 描述:设置GPIOx Pinx的中断服务函数
 * 参数
 *      @GPIOISRPort:
 *          GPIO中断的端口号:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO中断的端口Pin号:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @ISR_handler:
 *          中断服务函数，不允许存在返回值，不允许传入参数.
 * 返回值:void
 * 备注:
 */
void LIB_GPIO_ExtiIsrSet(uint8_t GPIOISRPort, uint8_t GPIOISRPin, void (*ISR_handler)(void))
{
    LIB_ISR_GPIOEXTIRegister(GPIOISRPort,GPIOISRPin,ISR_handler);
}
