#include <xc.h>

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt

    //LED
    ADCON1 = 0x0f;          // Set ADCON1 register for digital mode
    LATA = 0x00;            // Clear LATA
    TRISA = 0x00;
    TRISAbits.TRISA0 = 1;   // set RA0 as input
    
    //BUTTOM
    TRISB = 0x01;
    LATBbits.LATB0 = 0;
    
    INTCONbits.INT0IE = 0;
    INTCONbits.INT0IF = 0;
}

