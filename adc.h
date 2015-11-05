/*
=============================================================================
Copyright (C) 2015, Anonymous Maarten

This file is part of ADuC832 demoproject.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
=============================================================================
*/
#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

#include "ADUC832.h"

void adc_interrupt_handler(void) __interrupt INTERRUPT_ADCI __using 1;

void adc_init(void);

void adc_calibrate(void);

uint16_t adc_sample_channel(uint8_t channel);

#define ADC_CHANNEL_0           ADCCON2_CS_CHANNEL0_mask
#define ADC_CHANNEL_1           ADCCON2_CS_CHANNEL1_mask
#define ADC_CHANNEL_2           ADCCON2_CS_CHANNEL2_mask
#define ADC_CHANNEL_3           ADCCON2_CS_CHANNEL3_mask
#define ADC_CHANNEL_4           ADCCON2_CS_CHANNEL4_mask
#define ADC_CHANNEL_5           ADCCON2_CS_CHANNEL5_mask
#define ADC_CHANNEL_6           ADCCON2_CS_CHANNEL6_mask
#define ADC_CHANNEL_7           ADCCON2_CS_CHANNEL7_mask
#define ADC_CHANNEL_TEMPERATURE ADCCON2_CS_TEMPERATURE_mask
#define ADC_CHANNEL_DAC0        ADCCON2_CS_DAC0_mask
#define ADC_CHANNEL_DAC1        ADCCON2_CS_DAC1_mask
#define ADC_CHANNEL_AGND        ADCCON2_CS_AGND_mask
#define ADC_CHANNEL_VREF        ADCCON2_CS_VREF_mask

#endif
