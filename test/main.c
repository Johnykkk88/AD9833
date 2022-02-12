#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "AD9833.h"


    int main()
{
       // gpio_export(PIN_19);
        //gpio_export(PIN_23);
        //gpio_export(PIN_24);
        //(PIN_19, OUTPUT);
        //gpio_set_dir(PIN_23, OUTPUT);
        //gpio_set_dir(PIN_24, OUTPUT);
        //AD9833_Init(SQR,4000,0);
        //AD9833_SetWave(SIN);
        //AD9833_SetWave(SQR);
        AD9833_SetWaveData(4000,0);
        printf("Work");


    return 0;
}
