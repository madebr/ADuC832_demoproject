/*
=============================================================================
Copyright (C) 2015, Anonymous Maarten
Copyright (C) 2007, Jesus Calvino-Fraga / jesusc at ece.ubc.ca

This file is part of ADuC832 demoproject.

Based on ADuC84x.h, as shipped with sdcc.

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

#ifndef REG_ADUC832_H
#define REG_ADUC832_H

//Definition of the interrupt. Priority in comments.
#define INTERRUPT_PSMI      8   // (P1)  Power Supply Monitor Interrupt
#define INTERRUPT_WDS       9   // (P2)  Watchdog Timer Interrupt
#define INTERRUPT_IE0       0   // (P2)  External Interrupt 0
#define INTERRUPT_ADCI      6   // (P3)  ADC Interrupt
#define INTERRUPT_TF0       1   // (P4)  Timer/Counter 0 Interrupt
#define INTERRUPT_IE1       2   // (P5)  External Interrupt 1
#define INTERRUPT_TF1       3   // (P6)  Timer/Counter 1 Interrupt
#define INTERRUPT_ISPI_I2CI 7   // (P7)  SPI/I2C Interrupt
#define INTERRUPT_RI_TI     4   // (P8)  Serial Interrupt
#define INTERRUPT_TF2_EXF2  5   // (P9)  Timer/Counter 2 Interrupt
#define INTERRUPT_TII       10  // (P11) Time Interval Counter Interrupt (Buf in datasheet)

#include <compiler.h>

SFR(CFG832, 0xAF); // ADuC841 Configuration SFR.
  #define EPM2   0x10 // Flash/EE Controller and PWM Clock Frequency Configuration Bit 2.
  #define CFG832_EXSP    0x80 // Extended SP enable. Stack rolls over from
                              // 1: from 0x00ff to 0x0100
                              // 0: from 0x0000 to 0x0000
  #define CFG832_PWPO    0x40 // PWM pinout selection.
                              // 1: PWM output=P3.4 and P3.3
                              // 0: PWM output=P2.6 and P2.7
  #define CFG832_DBUG    0x20 // DAC output buffer
                              // 1: output buffer bypassed. 0: output buffer enabled.
  #define CFG832_EXTCLK  0x10 // 1:use external clock input on P3.4.
                              // 0:use internal PLL clock.
  #define CFG832_XRAMEN  0x01 // XRAM Enable Bit.

SFR(SP,    0x81); // Stack Pointer.
SFR(SPH,   0xB7); // Stack pointer high.

SFR16(DPTR, 0x82); // Data Pointer (16-bit)
SFR(DPL,   0x82); // Data Pointer Low.
SFR(DPH,   0x83); // Data Pointer High.
SFR(DPP,   0x84); // Data Pointer Page Byte.
SFR(DPCON, 0xA7); // Data Pointer Control SFR.
  #define DPCON_DPSEL 0x01 // Data pointer select. Appear in DPL, DPH and DPP SFRs.
                           // 1: shadow data pointer. Contents of this 24-bit register
                           // 0: main data pointer. Contents of a seperate 24-bit register
  #define DPCON_DP0   0x0b  //Main data pointer mode.
                            //0: 8052 behavior
                            //1: DPTR postincrement after MOVX or MOVC instruction.
                            //2: DPTR postdecrement after MOVX or MOVC instruction.
                            //3: DPTR LSB toggled after MOVX or MOVC instruction.
  #define DPCON_DP1   0x30  //Shadow data pointer mode.
                            //0: 8052 behavior
                            //1: DPTR postincrement after MOVX or MOVC instruction.
                            //2: DPTR postdecrement after MOVX or MOVC instruction.
                            //3: DPTR LSB toggled after MOVX or MOVC instruction.
  #define DPCON_DPT   0x40  //Data pointer automatic toggle enable.


SFR(PCON, 0x87); // Power Control.
  #define SMOD   0x80 //Double UART Baud Rate.
  //#define SERIPD 0x40 //I2C/SPI Power-Down Interrupt Enable.
  //#define INT0PD 0x20 //INT0 Power-Down Interrupt Enable.
  //#define ALEOFF 0x10 //Disable ALE Output.
  #define GF1    0x08 //General-Purpose Flag Bit.
  #define GF0    0x04 //General-Purpose Flag Bit.
  #define PD     0x02 //Power-Down Mode Enable.
  #define IDL    0x01 //Idle Mode Enable.

#define PCON_SMOD_mask 0x80
#define PCON_SERIPD_mask 0x40
#define PCON_INT0PD_mask 0x40
#define PCON_ALEOFF_mask 0x10
#define PCON_GF1_mask 0x04
#define PCON_GF0_mask 0x04
#define PCON_PD_mask 0x02
#define PCON_IDL_mask 0x01

SFR(TCON, 0x88); // Timer/Counter Control.
  SBIT(TF1, 0x88, 7); // Timer 1 overflow flag.
  SBIT(TR1, 0x88, 6); // Timer 1 run control flag.
  SBIT(TF0, 0x88, 5); // Timer 0 overflow flag.
  SBIT(TR0, 0x88, 4); // Timer 0 run control flag.
  SBIT(IE1, 0x88, 3); // Interrupt 1 flag.
  SBIT(IT1, 0x88, 2); // Interrupt 1 type control bit.
  SBIT(IE0, 0x88, 1); // Interrupt 0 flag.
  SBIT(IT0, 0x88, 0); // Interrupt 0 type control bit.

SFR(TMOD, 0x89); // Timer/Counter Mode Control.

#define TMOD_GATE_T1_mask 0x80 // Timer 1: External enable.
#define TMOD_CT_T1_mask   0x40 // Timer 1: Timer or counter select
#define TMOC_M_T1_mask    0x30 // Timer 1: Mode
#define TMOC_M_T1_shift   4
#define TMOD_GATE_T0_mask 0x08 // Timer 0: External enable.
#define TMOD_CT_T0_mask   0x04 // Timer 0: Timer or counter select
#define TMOD_M_T0_mask    0x03 // Timer 0: Mode
#define TMOD_M_T0_shift   0

SFR(TL0, 0x8A); // Timer 0 LSB.
SFR(TH0, 0x8C); // Timer 0 MSB.
SFR(TL1, 0x8B); // Timer 1 LSB.
SFR(TH1, 0x8D); // Timer 1 MSB.

SFR(T2CON, 0xC8); // Timer / Counter 2 Control.
  SBIT(TF2,   0xC8, 7); // Timer 2 overflow flag.
  SBIT(EXF2,  0xC8, 6); // Timer 2 external flag.
  SBIT(RCLK,  0xC8, 5); // Receive clock flag.
  SBIT(TCLK,  0xC8, 4); // Transmit clock flag.
  SBIT(EXEN2, 0xC8, 3); // Timer 2 external enable flag.
  SBIT(TR2,   0xC8, 2); // Start/stop control for timer 2.
  SBIT(CNT2,  0xC8, 1); // Timer or coutner select.
  SBIT(CAP2,  0xC8, 0); // Capture/reload flag.

SFR(RCAP2L, 0xCA); // Timer 2 Capture LSB.
SFR(RCAP2H, 0xCB); // Timer 2 Capture MSB.
SFR(TL2,    0xCC); // Timer 2 LSB.
SFR(TH2,    0xCD); // Timer 2 MSB.

SFR(P0, 0x80); // Port 0
  SBIT(P0_0, 0x80, 0); // Port 0 bit 0.
  SBIT(P0_1, 0x80, 1); // Port 0 bit 1.
  SBIT(P0_2, 0x80, 2); // Port 0 bit 2.
  SBIT(P0_3, 0x80, 3); // Port 0 bit 3.
  SBIT(P0_4, 0x80, 4); // Port 0 bit 4.
  SBIT(P0_5, 0x80, 5); // Port 0 bit 5.
  SBIT(P0_6, 0x80, 6); // Port 0 bit 6.
  SBIT(P0_7, 0x80, 7); // Port 0 bit 7.

SFR(P1, 0x90); // Port 1
  SBIT(P1_0, 0x90, 0); // Port 1 bit 0.
  SBIT(P1_1, 0x90, 1); // Port 1 bit 1.
  SBIT(P1_2, 0x90, 2); // Port 1 bit 2.
  SBIT(P1_3, 0x90, 3); // Port 1 bit 3.
  SBIT(P1_4, 0x90, 4); // Port 1 bit 4.
  SBIT(P1_5, 0x90, 5); // Port 1 bit 5.
  SBIT(P1_6, 0x90, 6); // Port 1 bit 6.
  SBIT(P1_7, 0x90, 7); // Port 1 bit 7.
  // Alternate names
  SBIT(T2EX, 0x90, 1); //Capture/reload trigger for Counter 2.
  SBIT(T2,   0x90, 0); //Input to Timer/Counter 2.

SFR(P2, 0xA0); // Port 2
   SBIT(P2_0, 0xA0, 0); // Port 2 bit 0.
   SBIT(P2_1, 0xA0, 1); // Port 2 bit 1.
   SBIT(P2_2, 0xA0, 2); // Port 2 bit 2.
   SBIT(P2_3, 0xA0, 3); // Port 2 bit 3.
   SBIT(P2_4, 0xA0, 4); // Port 2 bit 4.
   SBIT(P2_5, 0xA0, 5); // Port 2 bit 5.
   SBIT(P2_6, 0xA0, 6); // Port 2 bit 6.
   SBIT(P2_7, 0xA0, 7); // Port 2 bit 7.

SFR(P3, 0xB0); // Port 3
  SBIT(P3_0, 0xB0, 0); // Port 3 bit 0.
  SBIT(P3_1, 0xB0, 1); // Port 3 bit 1.
  SBIT(P3_2, 0xB0, 2); // Port 3 bit 2.
  SBIT(P3_3, 0xB0, 3); // Port 3 bit 3.
  SBIT(P3_4, 0xB0, 4); // Port 3 bit 4.
  SBIT(P3_5, 0xB0, 5); // Port 3 bit 5.
  SBIT(P3_6, 0xB0, 6); // Port 3 bit 6.
  SBIT(P3_7, 0xB0, 7); // Port 3 bit 7.
  // Alternate names.
  SBIT(RXD,  0xB0, 0);
  SBIT(TXD,  0xB0, 1);
  SBIT(INT0, 0xB0, 2);
  SBIT(INT1, 0xB0, 3);
  SBIT(T0,   0xB0, 4);
  SBIT(T1,   0xB0, 5);
  SBIT(WR,   0xB0, 6);
  SBIT(RD,   0xB0, 7);

SFR(I2CADD,  0x9B); // I2C Address Register.
SFR(I2CDAT,  0x9A); // I2C Data Register.

SFR(SCON, 0x98); // Serial Port Control.
  SBIT(SM0, 0x98, 7); // Serial Port Mode Bit 0: 9-bit baud rate
  SBIT(SM1, 0x98, 6); // Serial Port Mode Bit 1: Variable baud rate
  SBIT(SM2, 0x98, 5); // Serial Port Mode Bit 2: Multiprocessor communication enable
  SBIT(REN, 0x98, 4); // Enables serial reception.
  SBIT(TB8, 0x98, 3); // The 9th data bit that will be transmitted in Modes 2 and 3.
  SBIT(RB8, 0x98, 2); // In Modes 2 and 3, the 9th data bit that was received.
  SBIT(TI,  0x98, 1); // Transmit interrupt flag.
  SBIT(RI,  0x98, 0); // Receive interrupt flag.
#define SCON_SM0_mask 0x80
#define SCON_SM0_shift 7
#define SCON_SM1_mask 0x40
#define SCON_SM1_shift 6
#define SCON_SM2_mask 0x20
#define SCON_SM2_shift 5
  #define SCON_REN_mask 0x10
  #define SCON_TB8_mask 0x08
  #define SCON_RB8_mask 0x04
  #define SCON_TI_mask 0x02
  #define SCON_RI_mask 0x01

SFR(SBUF, 0x99); // Serial Data Buffer.

SFR(T3FD,  0x9D);       // Fractional divider ratio.
SFR(T3CON, 0x9E);       // T3CON is the baud rate control SFR, allowing Timer 3 to be
                        // used to set up the UART baud rate, and setting up the binary
                        // divider (DIV).
#define T3BAUDEN 0x80   // T3 UART BAUD Enable.
#define DIV2 0x04       // Binary Divider Factor bit 3.
#define DIV1 0x02       // Binary Divider Factor bit 2.
#define DIV0 0x01       // Binary Divider Factor bit 1.

#define T3CON_T3BAUDEN_shift 7
#define T3CON_T3BAUDEN_mask 0x80
#define T3CON_DIV_shift 0
#define T3CON_DIV_mask 0x07

SFR(TIMECON, 0xA1); // TIC Control Register. //FIXME: controleer datasheet
  #define TFH  0x40 // Twenty-Four Hour Select Bit.
  #define ITS1 0x20 // Interval Timebase Selection Bit 1.
  #define ITS0 0x10 // Interval Timebase Selection Bit 1.
  #define STI  0x08 // Single Time Interval Bit.
  #define TII  0x04 // TIC Interrupt Bit.
  #define TIEN 0x02 // Time Interval Enable Bit.
  #define TCEN 0x01 // Time Clock Enable Bit.

#define TIMECON_TFH_mask  0x40
#define TIMECON_ITS_mask  0x30
#define TIMECON_ITS_shift 4
#define TIMECON_STI_mask  0x08
#define TIMECON_TII_mask  0x04
#define TIMECON_TIEN_mask 0x02
#define TIMECON_TCEN_mask 0x01

#define TIMECON_ITS_1_128_SECONDS_mask (0x0 << TIMECON_ITS_shift)
#define TIMECON_ITS_SECONDS_mask (0x1 << TIMECON_ITS_shift)
#define TIMECON_ITS_MINUTES_mask (0x2 << TIMECON_ITS_shift)
#define TIMECON_ITS_HOURS_mask (0x3 << TIMECON_ITS_shift)

SFR(HTHSEC, 0xA2); // Hundredths Seconds Time Register.
SFR(SEC,    0xA3); // Seconds Time Register.
SFR(MIN,    0xA4); // Minutes Time Register.
SFR(HOUR,   0xA5); // Hours Time Register.
SFR(INTVAL, 0xA6); // User Time Interval Select Register.

SFR(IE,  0xA8); // Interrupt Enable.
  SBIT(EA,   0xA8, 7); // Global Interrupt Enable.
  SBIT(EADC, 0xA8, 6); // ADC Interrupt Enable.
  SBIT(ET2,  0xA8, 5); // Timer 2 Interrupt Enable.
  SBIT(ES,   0xA8, 4); // Serial Interrupt Enable.
  SBIT(ET1,  0xA8, 3); // Timer 1 Interrupt Enable.
  SBIT(EX1,  0xA8, 2); // External 1 Interrupt Enable.
  SBIT(ET0,  0xA8, 1); // Timer 0 Interrupt Enable.
  SBIT(EX0,  0xA8, 0); // External 0 Interrupt Enable.

SFR(IEIP2, 0xA9); // Secondary Interrupt Enable Register
  #define PTI   0x40 // Priority for time interval interrupt.
  #define PPSM  0x20 // Priority for power supply monitor interrupt.
  #define PSI2  0x10 // Priority for SPI/I2C interrupt. Same name as bit in IP???
  #define ETI   0x04 // Enable time interval counter interrupts.
  #define EPSMI 0x02 // Enable power supply monitor interrupts.
  #define ESI   0x01 // Enable SPI or I2C serial port interrupts.

#define IEIP2_PTI_mask 0x40
#define IEIP2_PPSM_mask 0x20
#define IEIP2_PSI2_mask 0x10
#define IEIP2_ETI_mask 0x04
#define IEIP2_EPSMI_mask 0x02
#define IEIP2_ESI_mask 0x01

SFR(PWMCON, 0xAE); //PWM control. //FIXME: controleer datasheet
  #define SNGL  0x80 // Turns off PMW output at P2.6 or P3.4.
  #define MD2   0x40 // PWM Mode Bit 2.
  #define MD1   0x20 // PWM Mode Bit 2.
  #define MD0   0x10 // PWM Mode Bit 2.
  #define CDIV1 0x08 // PWM Clock Divider bit 1.
  #define CDIV0 0x04 // PWM Clock Divider bit 1.
  #define CSEL1 0x02 // PWM Clock Select bit 1. (Typo in datasheet???)
  #define CSEL0 0x01 // PWM Clock Select bit 0.

SFR(PWM0L, 0xB1); // PWM 0 duty cycle low.
SFR(PWM0H, 0xB2); // PWM 0 duty cycle high.
SFR(PWM1L, 0xB3); // PWM 1 duty cycle low.
SFR(PWM1H, 0xB4); // PWM 1 duty cycle high.

SFR(IP,  0xB8); // Interrupt Priority Reg.
  SBIT(PSI,  0xB8, 7); // Priority for SPI/I2C interrupt. //FIXME: controleer datasheet? aanwezig?
  SBIT(PADC, 0xB8, 6); // ADC interrupt priority bit.
  SBIT(PT2,  0xB8, 5); // Timer 2 interrupt priority bit.
  SBIT(PS,   0xB8, 4); // Serial Port interrupt priority bit.
  SBIT(PT1,  0xB8, 3); // Timer 1 interrupt priority bit.
  SBIT(PX1,  0xB8, 2); // External interrupt 1 priority bit.
  SBIT(PT0,  0xB8, 1); // Timer 0 interrupt priority bit.
  SBIT(PX0,  0xB8, 0); // External interrupt 0 priority bit.

SFR(ECON,   0xB9); // Flash/EE Memory Control SFR.
SFR(EDATA1, 0xBC); // EE page data byte 1.
SFR(EDATA2, 0xBD); // EE page data byte 2.
SFR(EDATA3, 0xBE); // EE page data byte 3.
SFR(EDATA4, 0xBF); // EE page data byte 4.
SFR(EADRL,  0xC6); // EE address low.
SFR(EADRH,  0xC7); // EE address high.

SFR(WDCON, 0xC0); //Watchdog Timer Control Register.
  SBIT(PRE3, 0xC0, 7); // Timeout period bit 4.
  SBIT(PRE2, 0xC0, 6); // Timeout period bit 3.
  SBIT(PRE1, 0xC0, 5); // Timeout period bit 2.
  SBIT(PRE0, 0xC0, 4); // Timeout period bit 1.
  SBIT(WDIR, 0xC0, 3); // Watchdog Interrupt Response Enable Bit.
  SBIT(WDS,  0xC0, 2); // Watchdog Status Bit.
  SBIT(WDE,  0xC0, 1); // Watchdog Enable Bit.
  SBIT(WDWR, 0xC0, 0); // Watchdog Write Enable Bit.
  #define WDCON_PRE_mask   0xF0
  #define WDCON_PRE_shift  4
  #define WDCON_WDIR_mask  0x08
  #define WDCON_WDS_mask   0x04
  #define WDCON_WDE_mask   0x02
  #define WDCON_WDWR_mask  0x01

SFR(CHIPID, 0xC2); // System Self-Identification? WARNING: No description in the datasheet.

SFR(PSW, 0xD0); // Program Status Word.
   SBIT(CY,  0xD0, 7); // Carry Flag.
   SBIT(AC,  0xD0, 6); // Auxiliary Carry Flag.
   SBIT(F0,  0xD0, 5); // User-Defined Flag.
   SBIT(RS1, 0xD0, 4); // Register Bank Select 1.
   SBIT(RS0, 0xD0, 3); // Register Bank Select 0.
   SBIT(OV,  0xD0, 2); // Overflow Flag.
   SBIT(F1,  0xD0, 1); // User-Defined Flag.
   SBIT(P,   0xD0, 0); // Parity Flag.

SFR(DMAL,   0xD2); // DMA mode address pointer low.
SFR(DMAH,   0xD3); // DMA mode address pointer high.
SFR(DMAP,   0xD4); // DMA mode address pointer page. (?)
SFR(PLLCON, 0xD7); // PLL Control Register. //FIXME: controleer datasheet
  #define PLLCON_OSC_PD_mask   0x80 //Oscillator power-down bit
  #define PLLCON_LOCK_mask     0x40 //PLL lock bit. Read-only.
  #define PLLCON_FINT_mask     0x08 //Fast Interrupt Response Bit
  #define PLLCON_CD_mask       0x07 //CPU divider bits

SFR(PSMCON, 0xDF); // Power supply monitor.
  #define PSMCON_CMPD  0x40 // DVDD Comparator Bit.
  #define PSMCON_PSMI  0x20 // Power Supply Monitor Interrupt Bit.
  #define PSMCON_TPD1  0x10 // DVDD Trip Point Selection Bit 2.
  #define PSMCON_TPD0  0x08 // DVDD Trip Point Selection Bit 1.
  #define PSMCON_TPD   0x18 // DVDD Trip Point Selection
                            // 0 : 4.37V
                            // 1 : 3.08V
                            // 2 : 2.93V
                            // 3 : 2.63V
  #define PSMCON_PSMEN 0x01 // Power Supply Monitor Enable Bit.

SFR(ACC, 0xE0); // Accumulator
  SBIT(ACC_0, 0xE0, 0); // Accumulator bit 0.
  SBIT(ACC_1, 0xE0, 1); // Accumulator bit 1.
  SBIT(ACC_2, 0xE0, 2); // Accumulator bit 2.
  SBIT(ACC_3, 0xE0, 3); // Accumulator bit 3.
  SBIT(ACC_4, 0xE0, 4); // Accumulator bit 4.
  SBIT(ACC_5, 0xE0, 5); // Accumulator bit 5.
  SBIT(ACC_6, 0xE0, 6); // Accumulator bit 6.
  SBIT(ACC_7, 0xE0, 7); // Accumulator bit 7.

SFR(B,   0xF0); // B Register
  SBIT(B_0, 0xF0, 0); // Register B bit 0.
  SBIT(B_1, 0xF0, 1); // Register B bit 1.
  SBIT(B_2, 0xF0, 2); // Register B bit 2.
  SBIT(B_3, 0xF0, 3); // Register B bit 3.
  SBIT(B_4, 0xF0, 4); // Register B bit 4.
  SBIT(B_5, 0xF0, 5); // Register B bit 5.
  SBIT(B_6, 0xF0, 6); // Register B bit 6.
  SBIT(B_7, 0xF0, 7); // Register B bit 7.

SFR(I2CCON, 0xE8); // I2C Control Register
  // Master mode
  SBIT(MDO,    0xE8, 7); // I2C Software Master Data Output Bit.
  SBIT(MDE,    0xE8, 6); // I2C Software Master Data Output Enable Bit.
  SBIT(MCO,    0xE8, 5); // I2C Software Master Clock Output Bit.
  SBIT(MDI,    0xE8, 4); // I2C Software Master Data Input Bit.
  // Slave mode
  SBIT(I2CSI,  0xE8, 7); // I2C Stop Interrupt Enable Bit.
  SBIT(I2CGC,  0xE8, 6); // I2C General Call Status Bit.
  SBIT(I2CID1, 0xE8, 5); // I2C Interrupt Decode Bit 2.
  SBIT(I2CID0, 0xE8, 4); // I2C Interrupt Decode Bit 1.
  SBIT(I2CM,   0xE8, 3); // I2C Master/Slave Mode Bit.
  SBIT(I2CRS,  0xE8, 2); // I2C Reset Bit.
  SBIT(I2CTX,  0xE8, 1); // I2C Direction Transfer Bit.
  SBIT(I2CI,   0xE8, 0); // I2C Interrupt Bit.

SFR(ADCCON1,  0xEF); //ADC Control SFR 1 //FIXME: controleer datasheet
  //WARNING: bit 7 is named MD1 in the datasheet, but that name is already used.
  #define MD      0x80 // The mode bit selects the active operating mode of the ADC.
  #define EXT_REF 0x40 // Set by the user to select an external reference.
  #define CK1     0x20 // The ADC clock divide bit 1.
  #define CK0     0x10 // The ADC clock divide bit 0.
  #define AQ1     0x08 // ADC acquisition select bit 1.
  #define AQ0     0x04 // ADC acquisition select bit 0.
  #define T2C     0x02 // The Timer 2 conversion bit.
  #define EXC     0x01 // The external trigger enable bit.

#define ADCCON1_MD_mask 0x80
#define ADCCON1_EXT_REF_mask 0x40
#define ADCCON1_CK_shift 4
#define ADCCON1_CK_mask 0x30
#define ADCCON1_AQ_shift 2
#define ADCCON1_AQ_mask 0x0c
#define ADCCON1_T2C_mask 0x04
#define ADCCON1_EXC_mask 0x01

#define ADCCON1_CK_DIVIDE8_mask   (0x1 << ADCCON1_CK_shift)
#define ADCCON1_CK_DIVIDE4_mask   (0x2 << ADCCON1_CK_shift)
#define ADCCON1_CK_DIVIDE16_mask  (0x3 << ADCCON1_CK_shift)
#define ADCCON1_CK_DIVIDE32_mask  (0x4 << ADCCON1_CK_shift)
#define ADCCON1_AQ_1CLOCKCYCLE_mask  (0x0 << ADCCON1_AQ_shift)
#define ADCCON1_AQ_2CLOCKCYCLES_mask (0x1 << ADCCON1_AQ_shift)
#define ADCCON1_AQ_3CLOCKCYCLES_mask (0x2 << ADCCON1_AQ_shift)
#define ADCCON1_AQ_4CLOCKCYCLES_mask (0x3 << ADCCON1_AQ_shift)

SFR(ADCCON2,  0xD8); // ADC Control SFR 2.
  SBIT(ADCI,  0xD8, 7); // ADC Interrupt Bit.
  SBIT(DMA,   0xD8, 6); // DMA Mode Enable Bit.
  SBIT(CCONV, 0xD8, 5); // Continuous Conversion Bit.
  SBIT(SCONV, 0xD8, 4); // Single Conversion Bit.
  SBIT(CS3,   0xD8, 3); // Channel Selection Bit 4.
  SBIT(CS2,   0xD8, 2); // Channel Selection Bit 3.
  SBIT(CS1,   0xD8, 1); // Channel Selection Bit 2.
  SBIT(CS0,   0xD8, 0); // Channel Selection Bit 1.

#define ADCCON2_ADCI_mask 0x80
#define ADCCON2_DMA_mask 0x40
#define ADCCON2_CCONV_mask 0x20
#define ADCCON2_SCONV_mask 0x10
#define ADCCON2_CS_shift 0
#define ADCCON2_CS_mask 0x0f

#define ADCCON2_CS_CHANNEL0_mask     (0x0 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL1_mask     (0x1 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL2_mask     (0x2 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL3_mask     (0x3 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL4_mask     (0x4 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL5_mask     (0x5 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL6_mask     (0x6 << ADCCON2_CS_shift)
#define ADCCON2_CS_CHANNEL7_mask     (0x7 << ADCCON2_CS_shift)
#define ADCCON2_CS_TEMPERATURE_mask  (0x8 << ADCCON2_CS_shift)
#define ADCCON2_CS_DAC0_mask         (0x9 << ADCCON2_CS_shift)
#define ADCCON2_CS_DAC1_mask         (0xa << ADCCON2_CS_shift)
#define ADCCON2_CS_AGND_mask         (0xb << ADCCON2_CS_shift)
#define ADCCON2_CS_VREF_mask         (0xc << ADCCON2_CS_shift)
#define ADCCON2_CS_DMASTOP_mask      (0xf << ADCCON2_CS_shift)

SFR(ADCCON3,  0xF5); //ADC Control SFR 3 //FIXME: controleer datasheet
  #define BUSY    0x80 // ADC Busy Status Bit.
  #define AVGS1   0x20 // Number of Average Selection Bit 2.
  #define AVGS0   0x10 // Number of Average Selection Bit 1.
  #define TYPICAL 0x02 // Calibration Type Select Bit.
  #define SCAL    0x01 // Start Calibration Cycle Bit.

#define ADCCON3_BUSY_mask 0x80
#define ADCCON3_GNCLD_mask 0x40
#define ADCCON3_AVGS_mask 0x30
#define ADCCON3_AVGS_shift 4
#define ADCCON3_RSVD_mask 0x0a
#define ADCCON3_RSVD_shift 2
#define ADCCON3_TYPICAL_mask 0x2
#define ADCCON3_SCAL_mask 0x1

#define ADCCON3_AVGS_15SAMPLES_mask 0x00
#define ADCCON3_AVGS_1SAMPLE_mask   0x10
#define ADCCON3_AVGS_31SAMPLES_mask 0x20
#define ADCCON3_AVGS_63SAMPLES_mask 0x30

#define ADCCON3_RSVD_CALIBRATION 0x04
#define ADCCON3_TYPICAL_OFFSET_mask 0x02
#define ADCCON3_TYPICAL_GAIN_mask 0x00

SFR(ADCDATAL, 0xD9); // ADC Result byte low.
SFR(ADCDATAH, 0xDA); // ADC Result byte high.  WARNING: bits 4 to 7 are the ADC channel!
SFR(ADCOFSL,  0xF1); // Offset calibration coefficient byte (low).
SFR(ADCOFSH,  0xF2); // Offset calibration coefficient byte (high).
SFR(ADCGAINL, 0xF3); // Gain calibration coefficient byte (low).
SFR(ADCGAINH, 0xF4); // Gain calibration coefficient byte (high).

SFR(SPIDAT, 0xF7); // SPI Data Register.

SFR(SPICON, 0xF8); // SPI Control Register.
  SBIT(ISPI, 0xF8, 7); // SPI Interrupt Bit.
  SBIT(WCOL, 0xF8, 6); // Write Collision Error Bit.
  SBIT(SPE,  0xF8, 5); // SPI Interface Enable Bit.
  SBIT(SPIM, 0xF8, 4); // SPI Master/Slave Mode Select Bit.
  SBIT(CPOL, 0xF8, 3); // Clock Polarity Select Bit.
  SBIT(CPHA, 0xF8, 2); // Clock Phase Select Bit.
  SBIT(SPR1, 0xF8, 1); // SPI Bit Rate Select Bit 2.
  SBIT(SPR0, 0xF8, 0); // SPI Bit Rate Select Bit 1.

SFR(DAC0L,  0xF9); // DAC0 Data Low Byte.
SFR(DAC0H,  0xFA); // DAC0 Data High Byte.
SFR(DAC1L,  0xFB); // DAC1 Data Low Byte.
SFR(DAC1H,  0xFC); // DAC1 Data High Byte.
SFR(DACCON, 0xFD); // DAC Control Register. //FIXME: controleer datasheet
  #define MODE 0x80 // 0:12/1:8 bit selection.
  #define RNG1 0x40 // DAC1 Range Select Bit. (0:0-VREF)
  #define RNG0 0x20 // DAC0 Range Select Bit. (1:0-VDD)
  #define CLR1 0x10 // DAC1 Clear Bit. (0:output forced to 0)
  #define CLR0 0x08 // DAC0 Clear Bit. (1:output normal)
  #define SYNC 0x04 // DAC0/1 Update Synchronization Bit.
  #define PD1  0x02 // DAC1 Power-Down Bit. (1:power on)
  #define PD0  0x01 // DAC0 Power-Down Bit. (0:power off)

#endif //REG_ADUC832_H
