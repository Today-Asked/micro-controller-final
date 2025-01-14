#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000
// using namespace std;

#define left_setlow() LATAbits.LATA5 = 0;
#define right_setlow() LATAbits.LATA7 = 0;
#define toggle_left() LATAbits.LATA5 = !LATAbits.LATA5;
#define toggle_right() LATAbits.LATA7 = !LATAbits.LATA7;
#define brake_on() LATAbits.LATA6 = 1;
#define brake_off() LATAbits.LATA6 = 0;

int state = 0;

char str[20];

// void displayBinary(int num) {
//     // Assuming TRISD0-TRISD3 are connected to your LEDs (active low)
//     LATAbits.LATA1 = (num & 0x01) ? 1 : 0; // Least significant bit
//     LATAbits.LATA2 = (num & 0x02) ? 1 : 0;
//     LATAbits.LATA3 = (num & 0x04) ? 1 : 0;
//     LATAbits.LATA4 = (num & 0x08) ? 1 : 0; // Most significant bit
// }

void initialState(){
}

void ledRightOn(){
    dir = 1;
}

void ledOff(int num){
    dir = 0;
    left_setlow();
    right_setlow();
}

void ledLeftOn(int num){
    dir = -1;
}

void BrakeOn(){
    brake_on();
}

void BrakeOff(){
    brake_off();
}

void main(void) 
{    
    SYSTEM_Initialize() ; //need to be put after LED set, otherwile LED will not work
    
    while(1) {
        switch(state){
            case 0:
                initialState();
                break;
            case 1:
                ledRightOn();
                break;
            case 2:
                ledOff();
                break;
            case 3:
                ledLeftOn();
                break;
            case 4:
                BreakOn();
                break;
            case 5:
                BreakOff();
                break;
            default:
                break;
        }
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{
    
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
        if(command[0] == 'L' && command[1] == 'R' && command[2] == 'N' && strlen(command) == 3){
            state = 1;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == 'L' && command[1] == 'F' && strlen(command) == 2){
            state = 2;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == 'L' && command[1] == 'L' && command[2] == 'N' && strlen(command) == 3){
            state = 3;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == 'B' && command[1] == 'N' && strlen(command) == 2){
            state = 4;
            ClearBuffer();
            strcpy(command, "");
            return;
        }
        else if(command[0] == 'B' && command[1] == 'F' && strlen(command) == 2){
            state = 5;
            ClearBuffer();
            strcpy(command, "");
            return;
        }   
    }else if(TMR2IF){
        TMR2IF = 0;
        postpostscaler = (postpostscaler+1)%8;
        if(!postpostscaler){
            if(dir == -1){
                toggle_left();
            }else if(dir == 1){
                toggle_right();
            }else{}
        }
    }
    
   // process other interrupt sources here, if required

    return;
}