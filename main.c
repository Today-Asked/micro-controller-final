#include "adc.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

#define _XTAL_FREQ 4000000
#define LED LATAbits.LATA2
#define LED_DIR TRISAbits.TRISA2

void main(void)
{
    
    ADC_Initialize() ;
    
    LED_DIR = 0;
    LED = 0;
    
    int LED_index=0;
    int LED_Mem=0;
    
    while(1)
    {
        LED_index=ADC_Read(LED_index);
        
        if(LED_Mem<LED_index-5 || LED_Mem>LED_index+5)
        {
            LED_Mem=LED_index;
        }
        
        if(LED_Mem<=12)
        {
            LED = 1;
        }
        else
        {
            LED = 0;
        }
    }
    return;
}