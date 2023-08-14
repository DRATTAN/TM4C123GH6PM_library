/*
 * lib_ADCisrmanagement.c
 *
 *  Created on: 2023/07/19
 *      Author: RATTAN
 */
#include "lib_isrmanagement.h"

volatile uint8_t AdcSequenceSamplingFlage = 0;
volatile uint32_t AdcSequenceSamplingValueBuffer[8];

void ADC0S0IsrHandler(void)
{

    ADCIntClear(ADC0_BASE, 0);
    ADCSequenceDataGet(ADC0_BASE, 0, (uint32_t *)AdcSequenceSamplingValueBuffer);
    AdcSequenceSamplingFlage = 1;
}
