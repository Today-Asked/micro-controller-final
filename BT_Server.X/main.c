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
    
    SYSTEM_Initialize() ;
    
    int blinker_dir = 0;
    int prev_bend_sensor_val;
    
    while(1) {
        // if(isConnected == 1 && needSendMsg == 1){
        //     displayBinary(0);
        //     UART_Write_Text("LRN\r\n");
        //     needSendMsg = 0;
        // }

        Check_ADC(prev_bend_sensor_val);

        Check_Gyroscope(blinker_dir);
        __delay_ms(20);
    }
    return;
}
