#include "adc.h"
#include "ADUC832.h"

void adc_interrupt_handler(void) __interrupt INTERRUPT_ADCI __using 1
{
  ADCI = 0;
  P2_7 = 0x0;
}

//Configuration: ADCCON1, ADCCON2, ADCCON3
//DATA in ADCDATAH/ADCDATAHL[0:3]
//Channel in ADCDATAHL[4:7]
//Read P1.7/ADC7 (potmeter), Temperature Sensor, VREF
//Zin<61 Ohm
//Input Range: 0V to VREF!
//Set CLock divider such that: f=400kHz-4.5MHz

void adc_init(void)
{
  ADCCON1 = ADCCON1_MD_mask | (ADCCON1_CK_DIVIDE32_mask) |
    (ADCCON1_AQ_4CLOCKCYCLES_mask);
}

void adc_calibrate(void)
{
  //Calibrate device offset
  //ADCCON2 = ADCCON2_CS_AGND_mask;
  //ADCCON2 = ADCCON2_CS_TEMPERATURE_mask;
  ADCCON2 = ADCCON2_CS_CHANNEL5_mask;
  ADCCON3 = ADCCON3_AVGS_31SAMPLES_mask | ADCCON3_TYPICAL_OFFSET_mask | ADCCON3_SCAL_mask;
  do
  {
    B = ADCCON3;
  } while (B_7);

  //Calibrate device gain
  //ADCCON2 = ADCCON2_CS_VREF_mask;
  //ADCCON2 = ADCCON2_CS_TEMPERATURE_mask;
  ADCCON2 = ADCCON2_CS_CHANNEL6_mask;
  ADCCON3 = ADCCON3_AVGS_31SAMPLES_mask | ADCCON3_TYPICAL_GAIN_mask | ADCCON3_SCAL_mask;
  do
  {
    B = ADCCON3;
  } while (B_7);
}

#include "clock.h"

uint16_t adc_sample_channel(uint8_t channel)
{
  uint16_t sample;
  EADC = 0;
  ADCCON2 = channel| ADCCON2_SCONV_mask;
  CLOCK_BUSYWAIT_BIG_US(500);
  while(ADCI == 0);
  sample = (((uint16_t) (ADCDATAH & 0x0f)) << 8) | ADCDATAL;
  //EADC = 0;
  return sample;
}
