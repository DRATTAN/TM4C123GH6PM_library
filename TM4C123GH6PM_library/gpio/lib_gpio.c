/*EK-TM4C123GXL_LIB
 * ����:RATTAN
 * �ļ�����:none
 */

#include "lib_gpio.h"


void LIB_GPIO_UnlockAllPins()
{
    GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOUnlockPin(GPIO_PORTD_BASE, GPIO_PIN_7);
}


/*
 * ����:��ʼ��GPIO�˿�
 * ����
 *  @Port:
 *      GPIO����ַ:
 *          GPIOA-GPIOF
 *  @Pin:
 *      �˿ں�:
 *          GPIO_PIN_0-GPIO_PIN_7
 *  @Dir:
 *      ����:
 *          GPIO_DIR_OUTPUT     ���,
 *          GPIO_DIR_INPUT      ����
 *  @Type:
 *      �� ��/�� ����,��Pin����Ϊ����ʱ,��������Ϊ:
 *          GPIO_PIN_TYPE_INPUT_WPU     ������
 *          GPIO_PIN_TYPE_INPUT_WPD     ������
 *      ������PinΪ���ʱ,��������Ϊ:
 *          GPIO_PIN_TYPE_OUTPUT_STD    �������
 *          GPIO_PIN_TYPE_OUTPUT_OD     ��©���
 *  @Value:
 *      Ĭ�������ƽ,�� Dir Ϊ GPIO_DIR_OUTPUT ʱ��Ч,��������Ϊ:
 *          GPIO_PIN_SET        �ߵ�ƽ
 *          GPIO_PIN_RESET      �͵�ƽ
 *      �� Dir Ϊ GPIO_DIR_OUTPUT ʱ,�˲����ᱻ����,��������0x00
 *
 * ����ֵ:void
 * ��ע:����ͬʱ��ʼ�������ͬGPIOx�µĶ˿�,����
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
 * ����:��GPIO��PINд������(���õ�ƽ)
 * ����
 *  @Port:
 *      GPIO����ַ:
 *          GPIOA-GPIOF
 *  @Pin:
 *      �˿ں�:
 *          GPIO_PIN_0-GPIO_PIN_7
 *  @Value:
 *      ���õĵ�ƽ,��������Ϊ:
 *      GPIO_PIN_SET        ����ߵ�ƽ
 *      GPIO_SET_RESET      ����͵�ƽ
 * ����ֵ:void
 * ��ע:����ͬʱ�����ͬGPIOx����˿��µ�Pin����ͬ��ѹ,����LIB_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
 */
void LIB_GPIO_WritePin(uint32_t Port, uint32_t Pin, uint8_t Value)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + (Pin << 2)));
    if(GPIO_PIN_RESET == Value) HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg & ~((uint8_t)Pin));
    else if(GPIO_PIN_SET == Value) HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg | (uint8_t)Pin);
    else return;
}


/*
 * ����:��ȡGPIO�˿ڵ�Pin�ĵ�ƽ
 * ����
 *  @Port:
 *      GPIO����ַ:
 *          GPIOA-GPIOF
 *  @Pin:
 *      �˿ں�:
 *          GPIO_PIN_0-GPIO_PIN_7
 * ����ֵ:uint8_t
 *          ����ֵΪ��ȡ���ĵ�ƽ��ֵ.
 * ��ע:����ͬʱ��ȡ��ͬGPIOx�µĶ��Pin��ֵ,����LIB_GPIO_ReadPin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);����ЩPin�����κ�
 * һ��ΪGPIO_PIN_SETʱ�򷵻�GPIO_PIN_SET,���򷵻�GPIO_PIN_RESET.����һ��ֻ��ȡһ��Pin
 */
uint8_t LIB_GPIO_ReadPin(uint32_t Port, uint32_t Pin)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + (Pin << 2))) & (uint8_t)Pin;
    if(reg) return GPIO_PIN_SET;
    else return GPIO_PIN_RESET;
}

/*
 * ����:��תGPIO�˿�Pin�ĵ�ƽ
 * ����
 *  @Port:
 *      GPIO����ַ:
 *          GPIOA-GPIOF
 *  @Pin:
 *      �˿ں�:
 *          GPIO_PIN_0
 * ����ֵ:void
 * ��ע:����ͬʱ��תGPIOx�µĶ��Pin�ĵ�ƽ,����LIB_GPIO_TogglePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);ÿ��Pin�ĵĵ�ƽ�ᱻ�ֱ�ȡ����д��
 */
void LIB_GPIO_TogglePin(uint32_t Port, uint32_t Pin)
{
    uint8_t reg = HWREG(Port + (GPIO_O_DATA + ((uint8_t)Pin << 2)));
    HWREG(Port + (GPIO_O_DATA + (Pin << 2))) = (reg ^ (uint8_t)Pin);
}


/*
 * ����:��ʼ��GPIOx�µ���Pinx���ж�
 * ����
 *      @GPIOISRPort:
 *          GPIO�жϵĶ˿ں�:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO�жϵĶ˿�Pin��:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @Trigger:
 *          ������ʽ����ѡ�Ĳ�����:
 *               GPIO_EXTI_TRIGGER_RISING �����ش���
 *               GPIO_EXTI_TRIGGER_FALLING �½��ش���
 *               GPIO_EXTI_TRIGGER_BOTHEDGES ˫���ش���
 *               GPIO_EXTI_TRIGGER_HIGH �ߵ�ƽ����
 *               GPIO_EXTI_TRIGGER_LOW �͵�ƽ����
 *               GPIO_EXTI_TRIGGER_DISCRETE_INT ��ɢ����
 *      @ISR_handler:
 *          �жϻص���������������ڷ���ֵ�������������.
 * ����ֵ:void
 * ��ע:(2023/05/14) @param(Trigger) GPIO_EXTI_TRIGGER_DISCRETE_INTδʵ��,����ʹ��.
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
 * ����:��/�ر�GPIOx ��ĳ��Pin���ⲿ�ж�
 * ����
 *      @GPIOISRPort:
 *          GPIO�жϵĶ˿ں�:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO�жϵĶ˿�Pin��:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @Ctrl:
 *          ��/�ر�:
 *               GPIO_EXTI_CTRL_ENABLE ��
 *               GPIO_EXTI_CTRL_DISABLE �ر�
 *      @ISR_handler:
 *          �жϻص���������������ڷ���ֵ�������������.
 * ����ֵ:void
 * ��ע:
 */
void LIB_GPIO_ExtiCtrlPin(uint8_t GPIOISRPort, uint8_t GPIOISRPin, uint8_t Ctrl)
{
    uint32_t GPIO_BASE_TABLE[6] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
    if(GPIO_EXTI_CTRL_ENABLE == Ctrl) GPIOIntEnable(GPIO_BASE_TABLE[GPIOISRPort], 0x01 << GPIOISRPin);
    if(GPIO_EXTI_CTRL_DISABLE == Ctrl) GPIOIntDisable(GPIO_BASE_TABLE[GPIOISRPort], 0x01 << GPIOISRPin);
}

/*
 * ����:��/�ر�GPIOx���ⲿ�ж�
 * ����
 *      @GPIOISRPort:
 *          GPIO�жϵĶ˿ں�:
 *              ISR_GPIOA-ISR_GPIOF
 *      @Ctrl:
 *          ��/�ر�:
 *               GPIO_EXTI_CTRL_ENABLE ��
 *               GPIO_EXTI_CTRL_DISABLE �ر�
 *      @ISR_handler:
 *          �жϻص���������������ڷ���ֵ�������������.
 * ����ֵ:void
 * ��ע:
 */
void LIB_GPIO_ExtiCtrlPort(uint8_t GPIOISRPort, uint8_t Ctrl)
{
    uint32_t GPIO_ASSIGNMENTS_TABLE[6] = {INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF};
    if(GPIO_EXTI_CTRL_ENABLE == Ctrl) IntEnable(GPIO_ASSIGNMENTS_TABLE[GPIOISRPort]);
    if(GPIO_EXTI_CTRL_DISABLE == Ctrl) IntDisable(GPIO_ASSIGNMENTS_TABLE[GPIOISRPort]);
}

/*
 * ����:����GPIOx Pinx���жϷ�����
 * ����
 *      @GPIOISRPort:
 *          GPIO�жϵĶ˿ں�:
 *              ISR_GPIOA-ISR_GPIOF
 *      @GPIOISRPin:
 *          GPIO�жϵĶ˿�Pin��:
 *              ISR_GPIO_PIN_0-ISR_GPIO_PIN_7
 *      @ISR_handler:
 *          �жϷ���������������ڷ���ֵ�������������.
 * ����ֵ:void
 * ��ע:
 */
void LIB_GPIO_ExtiIsrSet(uint8_t GPIOISRPort, uint8_t GPIOISRPin, void (*ISR_handler)(void))
{
    LIB_ISR_GPIOEXTIRegister(GPIOISRPort,GPIOISRPin,ISR_handler);
}
