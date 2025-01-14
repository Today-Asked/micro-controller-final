#include <xc.h>
#include <stdio.h>
#include "uart.h"

#define BRAKE_ON() LATAbits.LATA7 = 1;
#define BRAKE_OFF() LATAbits.LATA7 = 0;

void ADC_Initialize(void)
{
    TRISAbits.RA0 = 1;
    
    ADCON1 = 0x0e;//AN0

    ADFM = 1 ;// Right Justifie
    ADCON2bits.ADCS = 7;//500 kHz
    ADCON2bits.ACQT = 0b001;//Tad = 2 us
    // Flush ADC output Register
    ADRESH=0;
    ADRESL=0;
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    
    return(digital);
}

void Check_ADC(int prev_sensor_val){

    int bend_sensor_val=ADC_Read(bend_sensor_val);
    
    if(prev_sensor_val<bend_sensor_val-5 || prev_sensor_val>bend_sensor_val+5)
    {
        prev_sensor_val=bend_sensor_val;
    }
    
    if(prev_sensor_val<=0x0130)
    {
        BRAKE_ON();
    }
    else if(prev_sensor_val>=0x0170)
    {
        BRAKE_OFF();
    }
}