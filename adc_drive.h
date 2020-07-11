/*
 * adc_drive.h
 *
 * Created: 5/21/2020 3:03:30 PM
 * Author : mosad
 */ 
#ifndef ADC_DRIVE_H_
#define ADC_DRIVE_H_

#include <xc.h>
#include "Main.h"
#include "macros.h"

void ADC_Init(void);
uint16_t ADC_Read (uint8_t channel);
		
#endif 