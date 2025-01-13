#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

int count = 0;
int isConnected = 0;
int needSendMsg = 0;

char str[20];

void displayBinary(int num) {
    // Assuming TRISD0-TRISD3 are connected to your LEDs (active low)
    LATAbits.LATA1 = (num & 0x01) ? 1 : 0; // Least significant bit
    LATAbits.LATA2 = (num & 0x02) ? 1 : 0;
    LATAbits.LATA3 = (num & 0x04) ? 1 : 0;
    LATAbits.LATA4 = (num & 0x08) ? 1 : 0; // Most significant bit
}

void main(void) 
{
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
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    
    SYSTEM_Initialize() ;
    
    
    while(1) {
        if(isConnected == 1 && needSendMsg == 1){
            displayBinary(0);
            UART_Write_Text("LRN\r\n");
            needSendMsg = 0;
        }
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{
    if (INTCONbits.RBIF) {
        __delay_ms(100);
        if(count == 0){
            UART_Write_Text("AT\r\n");
        }
        else if(count == 1){
            UART_Write_Text("AT+INQ\r\n");
        }
        else if(count == 2){
            UART_Write_Text("AT+CONN1\r\n");
            count = -1;
        }
        count++;
        
        INTCONbits.INT0IF = 0;
    }
}

// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
        char command[20];
        if(RCREG == '\r' || RCREG == '\n'){
            strcpy(command, GetString());
            ClearBuffer();
        }
        if(command[0] == 'O' && command[1] == 'K' && strlen(command) == 2){
            LATAbits.LATA1 = 1;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == '+' && command[1] == 'I' && command[2] == 'N' && command[3] == 'Q' && command[4] == 'E'){
            LATAbits.LATA2 = 1;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == '+' && command[1] == 'C' && command[2] == 'o' && command[3] == 'n'){
            LATAbits.LATA3 = 1;
            PIE1bits.RCIE = 0;
            isConnected = 1;
            needSendMsg = 1;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
    }
    
   // process other interrupt sources here, if required
    
    return;
}