/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#include "AD9833.h"
// ------------------- Variables ----------------
        uint8_t _waveform = WAVEFORM_SINE;
        uint8_t _sleep_mode = NO_POWERDOWN;
        uint8_t _freq_source = 0;
        uint8_t _phase_source = 0;
        uint8_t _reset_state = 0;

// -------------------------------- Functions --------------------------------
// -------------------------------- Chip Select --------------------------------
void AD9833_Select(void)
{
        gpio_export(AD9833SS);
        gpio_set_dir(AD9833SS, OUTPUT);
        gpio_write(AD9833SS, LOW);
}
// -------------------------------- Chip Unselect --------------------------------
void AD9833_Unselect(void)
{
        gpio_export(AD9833SS);
        gpio_set_dir(AD9833SS, OUTPUT);
        gpio_write(AD9833SS, HIGH);
}
// -------------------------------- Init --------------------------------
void AD9833_Init(WaveDef Wave, uint32_t freq, uint16_t phase_deg)
{
        AD9833_OutputEnable(0);
        AD9833_SetWaveform(Wave);
        AD9833_WriteCfgReg();
        AD9833_SetFrequency(freq);
        AD9833_SetPhase(phase_deg);
        AD9833_OutputEnable(1);
}
// -------------------------------- Output Enable --------------------------------
void AD9833_OutputEnable(uint8_t output_state)
{
        _reset_state = !output_state;
        AD9833_WriteCfgReg();
}
// -------------------------------- Write Config Register --------------------------------
void AD9833_WriteCfgReg(void)
{
        uint16_t cfg = 0;
        cfg |= _waveform;
        cfg |= _sleep_mode;
        cfg |= _waveform;
        cfg |= (_freq_source ? FSELECT_DB11 : 0);
        cfg |= (_phase_source ? PSELECT_DB10 : 0);
        cfg |= (_reset_state ? RESET_DB8 : 0);
        cfg |= B28_DB13;
        AD9833_WriteRegister(cfg);
}
// -------------------------------- Write Register --------------------------------
void AD9833_WriteRegister(uint16_t data)
{
        AD9833_Select();
        //uint8_t LByte = data & 0xff;
        //uint8_t LByte = (data>>8) & 0xff;

        //Rewrite!


        /*for (uint8_t i = 0; i < 16 ; i++) {
          if(data & 0x8000) gpio_write(AD9833DATA, HIGH);   //bit=1, Set High
		else gpio_write(AD9833DATA, HIGH);        //bit=0, Set Low
		ASM_NOP();
		gpio_write(AD9833SCK, LOW);             //Data is valid on falling edge
		ASM_NOP();
		gpio_write(AD9833SCK, HIGH);
		data = data<<1; //Shift left by 1 bit
        }
	gpio_write(AD9833DATA, LOW);                    //Idle low
	ASM_NOP();
	*/
}
// -------------------------------- Set Waveform --------------------------------
void AD9833_SetWaveform(WaveDef Wave)
{
        if (Wave == wave_sine)           _waveform = WAVEFORM_SINE;
        else if (Wave == wave_square)    _waveform = WAVEFORM_SQUARE;
        else if (Wave == wave_triangle)  _waveform = WAVEFORM_TRIANGLE;
        AD9833_WriteCfgReg();
}
// -------------------------------- Set Frequency --------------------------------
void AD9833_SetFrequency(uint32_t freq)
{
        if (freq > (FMCLK >> 1)) freq = FMCLK >> 1;//bitwise FMCLK/2
        else if (freq < 0) freq = 0;

        uint32_t freq_reg = (float)freq * (float)((1<<28)/FMCLK);

        uint16_t LSB = FREQ0_REG | (freq_reg & 0x3FFF);
        uint16_t MSB = FREQ0_REG | (freq_reg >> 14);

        AD9833_WriteCfgReg();
        AD9833_WriteRegister(LSB);
        AD9833_WriteRegister(MSB);
}
// -------------------------------- Set Phase --------------------------------
void AD9833_SetPhase(uint16_t phase_deg)
{
        if (phase_deg < 0) phase_deg = 0;
        else if (phase_deg > 360) phase_deg %= 360;

        uint16_t phase_val = ((uint16_t)(phase_deg * BITS_PER_DEG)) & 0xFFF;
        AD9833_WriteRegister(PHASE0_REG | phase_val);
}
// -------------------------------- Sleep Mode --------------------------------
void AD9833_SleepMode(uint8_t mode)
{
       _sleep_mode = mode;
       AD9833_WriteCfgReg();
}



