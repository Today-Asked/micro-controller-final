#include <xc.h>

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt

    //LED
    ADCON1 = 0x0f;          // Set ADCON1 register for digital mode
    LATA = 0x00;            // Clear LATA
    TRISAbits.TRISA2 = 0; 
    TRISAbits.TRISA5 = 0;   // Set RA1 as output
    TRISAbits.TRISA6 = 0;   // Set RA2 as output
    TRISAbits.TRISA7 = 0;   // Set RA3 as output
}

