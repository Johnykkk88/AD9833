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

        //Remake
        uint16_t FRQLW = 0;    // MSB of Frequency Tuning Word
        uint16_t FRQHW = 0;    // LSB of Frequency Tuning Word
        uint32_t phaseVal=0;  // Phase Tuning Value
        uint8_t WKNOWN=0;      // Flag Variable



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
void AD9833_Init(WaveDef wave, uint32_t freq, uint16_t phase_deg)
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
        uint8_t LByte = data & 0xff;
        uint8_t LByte = (data>>8) & 0xff;

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
// ------------------------------------------------ Sets Output Wave Type
void AD9833_SetWave(uint16_t Wave){
  switch(Wave){
  case 0:
  gpio_write(AD9833SS, LOW);
    writeSPI(0x2000); // Value for Sinusoidal Wave
     gpio_write(AD9833SS, HIGH);
    WKNOWN=0;
    break;
  case 1:
     gpio_write(AD9833SS, LOW);
    writeSPI(0x2028); // Value for Square Wave
    gpio_write(AD9833SS, HIGH);
    WKNOWN=1;
    break;
  case 2:
        gpio_write(AD9833SS, LOW);
    writeSPI(0x2002); // Value for Triangle Wave
    gpio_write(AD9833SS, HIGH);
    WKNOWN=2;
    break;
  default:
    break;
  }
}

// ------------------------------------------------ Sets Wave Frequency & Phase (In Degree) In PHASE0 & FREQ0 Registers
void AD9833_SetWaveData(float Frequency,float Phase){
ASM_NOP();
 // ---------- Tuning Word for Phase ( 0 - 360 Degree )
 if(Phase<0)Phase=0; // Changing Phase Value to Positive
 if(Phase>360)Phase=360; // Maximum value For Phase (In Degree)
 phaseVal  = ((int)(Phase*(4096/360)))|0XC000;  // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address

 // ---------- Tuning word for Frequency
long freq=0;
freq=(int)(((Frequency*pow(2,28))/FMCLK)+1); // Tuning Word
FRQHW=(int)((freq & 0xFFFC000) >> 14); // FREQ MSB
FRQLW=(int)(freq & 0x3FFF);  // FREQ LSB
FRQLW |= 0x4000;
FRQHW |= 0x4000;
 // ------------------------------------------------ Writing DATA
 gpio_write(AD9833DATA, HIGH);
 gpio_write(AD9833SCK, HIGH);
 gpio_write(AD9833SS, HIGH);
 gpio_write(AD9833SS, LOW); //low = selected
	ASM_NOP();
	writeSPI(0x2100); // enable 16bit words and set reset bit
	writeSPI(FRQLW);
	writeSPI(FRQHW);
        writeSPI(phaseVal);
	writeSPI(0x2000); // clear reset bit
	ASM_NOP();
	gpio_write(AD9833SS, HIGH); //high = deselected
AD9833_SetWave(WKNOWN);
ASM_NOP();
return;
}






