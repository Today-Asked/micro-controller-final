#include <xc.h>

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt

    //LED
    ADCON1 = 0x0f;          // Set ADCON1 register for digital mode
    LATA = 0x00;            // Clear LATA
    TRISAbits.TRISA1 = 0;   // Set RA1 as output
    TRISAbits.TRISA2 = 0;   // Set RA2 as output
    TRISAbits.TRISA3 = 0;   // Set RA3 as output
    TRISAbits.TRISA4 = 0;   // Set RA4 as output
    LATAbits.LATA1 = 0;    // RA1 OFF
    LATAbits.LATA2 = 0;    // RA2 OFF
    LATAbits.LATA3 = 0;    // RA3 OFF
    LATAbits.LATA4 = 0;    // RA4 OFF
    
    //BUTTOM
    TRISB = 0x01;
    LATBbits.LATB0 = 0;
    
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
}

