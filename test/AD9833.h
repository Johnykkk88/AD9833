/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef _AD_9833_H
#define _AD_9833_H

#include <math.h>
#include <stdint.h>
#include "gpio.h"

// --------------------------------- Defines -------------------------
#define AD9833_SPI_PORT MOSI
#define AD9833DATA PIN_19     // SPI DATA PIN
#define AD9833SCK PIN_23      // SPI Clock PIN
#define AD9833SS PIN_24       // SPI Chip Select
#define ASM_NOP() asm("NOP")  // Assembly NOPE (Little Delay)
// -------------------------------- Control Registers Bits --------------------------------
#define MODE_DB1                (1<<1)
#define DIV2_DB3                (1<<3)
#define OPBITEN_DB5             (1<<5)
#define SLEEP12_DB6             (1<<6)
#define SLEEP1_DB7              (1<<7)
#define RESET_DB8               (1<<8)
#define PSELECT_DB10            (1<<10)
#define FSELECT_DB11            (1<<11)
#define HLB_DB12                (1<<12)
#define B28_DB13                (1<<13)
// -------------------------------- Bitmask to register access --------------------------------
#define FREQ0_REG               0x4000
#define PHASE0_REG              0xC000
// -------------------------------- Waveform Types --------------------------------
#define WAVEFORM_SINE           0
#define WAVEFORM_TRIANGLE       MODE_DB1
#define WAVEFORM_SQUARE         OPBITEN_DB5 | DIV2_DB3
#define WAVEFORM_SQUARE_DIV2    OPBITEN_DB5
// -------------------------------- Sleep Modes --------------------------------
#define NO_POWERDOWN            0
#define DAC_POWERDOWN           SLEEP12_DB6
#define CLOCK_POWERDOWN         SLEEP1_DB7
#define FULL_POWERDOWN          SLEEP12_DB6 | SLEEP1_DB7

#define FMCLK                   25000000
#define BITS_PER_DEG            11.3777777777778 //4096/360


typedef enum {
        wave_sine,
        wave_square,
        wave_triangle
}WaveDef;

// ------------------ Functions  ---------------------
void AD9833_Select(void);
void AD9833_Unselect(void);
void AD9833_Init(WaveDef Wave, uint32_t freq, uint16_t phase_deg);
void AD9833_OutputEnable(uint8_t output_state);
void AD9833_WriteCfgReg(void);
void AD9833_WriteRegister(uint16_t data);
void AD9833_SetWaveform(WaveDef Wave);
void AD9833_SetFrequency(uint32_t freq);
void AD9833_SetPhase(uint16_t phase_deg);
void AD9833_SleepMode(uint8_t mode);

#endif
