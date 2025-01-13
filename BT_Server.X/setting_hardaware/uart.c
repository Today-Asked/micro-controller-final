#include <xc.h>
    //setting TX/RX
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

char mystring[20];
int lenStr = 0;

void UART_Initialize() {
           
    /*TODObasic   
           Serial Setting      
        1.   Setting Baud rate
        2.   choose sync/async mode 
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
     */
           
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate
    TXSTAbits.SYNC = 0;           
    BAUDCONbits.BRG16 = 0;          
    TXSTAbits.BRGH = 1;
    SPBRG = 25; //38400      
    
   //   Serial enable
    RCSTAbits.SPEN = 1;          
    PIR1bits.TXIF = 0; //interrupt
    PIR1bits.RCIF = 1; //receiver changed
    TXSTAbits.TXEN = 1;           
    RCSTAbits.CREN = 1; //receiver    
    PIE1bits.TXIE = 0; //interrupt changed   
    IPR1bits.TXIP = 0;             
    PIE1bits.RCIE = 1; //interrupt         
    IPR1bits.RCIP = 0; //interrupt   
    
    
    }

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}


void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    /* TODObasic: try to use UART_Write to finish this function */
    //while (!PIR1bits.RCIF);
    unsigned char receivedData = RCREG;
    if(receivedData == '\r'){
        UART_Write_Text("\r\n");
        return;
    }
    mystring[lenStr] = receivedData;
    lenStr++;
    UART_Write(receivedData);
    return ;
}

char *GetString(){
    return mystring;
}

