#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

#define _XTAL_FREQ 4000000
#define LED LATAbits.LATA2
#define LED_DIR TRISAbits.TRISA2

//PWM
//#define _XTAL_FREQ 125000

//__delay_ms(500)=0.5S

int index,state;
int count,hundreds, tens, ones;
char str[20];

unsigned int duty_cycle = 0x04;
unsigned int duty_low = 0x00;
unsigned int clockwise = 0x00; // 0 = counterclockwise, 1 = clockwise

void Mode1() // Todo : Mode1 
{
    //UART
    ClearBuffer();
    str[0] = 'M';
    str[1] = 'o';
    str[2] = 'd';
    str[3] = 'e';
    str[4] = '1';
    str[5] = '\n';
    str[6] = '\r';
    str[7] = '\0';
    UART_Write_Text(str);
    ClearBuffer();
    
    count=0;
    index=0;
    
    while(1)
    {
        strcpy(str, GetString());
        for(int i=0;str[i]=='0'||str[i]=='1'||str[i]=='2'||str[i]=='3'||str[i]=='4'||str[i]=='5'||str[i]=='6'||str[i]=='7'||str[i]=='8'||str[i]=='9'||str[i]==' '||str[i]=='\n'||str[i]=='\r';i++)
        {
            if(str[i]=='\r')
            {
                count=str[0]-48;
                index=str[2]-48;
                
                ClearBuffer();
                str[0] = '\r';
                str[1] = '\n';
                str[2] = '\0';
                UART_Write_Text(str);
                ClearBuffer();
                break;
            }
        }
        
        if(count==1)
        {
            __delay_ms(250);
        }
        else if(count==2)
        {
            __delay_ms(500);
        }
        else if(count==3)
        {
            __delay_ms(1000);
        }
        
        if(index==7)
        {
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            index=index-1;
        }
        else if(index==6)
        {
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 0;
            index=index-1;
        }
        else if(index==5)
        {
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;
            index=index-1;
        }
        else if(index==4)
        {
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
            index=index-1;
        }
        else if(index==3)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 1;
            index=index-1;
        }
        else if(index==2)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 0;
            index=index-1;
        }
        else if(index==1)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;
            index=index-1;
        }
        else if(index==0)
        {
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 0;
        }
        
        
        /*strcpy(str, GetString());
        if(str[0]!='\0')
        {
            if(str[0]=='1')
            {
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
            }
            else if(str[0]=='2')
            {
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 1;
            }
            else if(str[0]=='e') 
            {
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                break;
            }
            ClearBuffer();
        }*/
    }
    return ;
}
void Mode2()// Todo : Mode2
{ 
    //ADC
    //Q3b
    ClearBuffer();
    str[0] = 'M';
    str[1] = 'o';
    str[2] = 'd';
    str[3] = 'e';
    str[4] = '2';
    str[5] = '\n';
    str[6] = '\r';
    str[7] = '\0';
    UART_Write_Text(str);
    ClearBuffer();
    
    int temp;
    int memX=0;
    int memindex=0;
    int x=20;
    
    while(1)
    {
        strcpy(str, GetString());
        temp=0;
        for(int i=0;str[i]=='0'||str[i]=='1'||str[i]=='2'||str[i]=='3'||str[i]=='4'||str[i]=='5'||str[i]=='6'||str[i]=='7'||str[i]=='8'||str[i]=='9'||str[i]=='\n'||str[i]=='\r';i++)
        {
            if(str[i]=='\r')
            {
                ClearBuffer();
                str[0] = '\r';
                str[1] = '\n';
                str[2] = '\0';
                UART_Write_Text(str);
                ClearBuffer();
                if(temp!=x && (temp>=10 && temp<=600))
                {
                    x=temp;
                }
                break;
            }
            temp=(temp*10)+str[i]-48;
        }
        
        index=ADC_Read(index);
        
        if(memX!=x || (memindex<index-5 || memindex>index+5))
        {
            memX=x;
            memindex=index;
            
            count=index;
            hundreds=count/100;
            tens=count/10;
            ones=count%10;
            if(hundreds==0)
            {
                if(tens==0)
                {
                    UART_Write(48+ones);
                }
                else
                {
                    UART_Write(48+tens);
                    UART_Write(48+ones);
                }
            }
            else
            {
                UART_Write(48+hundreds);
                UART_Write(48+tens);
                UART_Write(48+ones);
            }
            str[0] = '\r';
            str[1] = '\n';
            str[2] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
        }
        
        if(count<=12)
        {
            LATAbits.LATA2 = 1;
        }
        else
        {
            LATAbits.LATA2 = 0;
        }
        
        /*if(str[0]!='\0')
        {
            if(str[0]=='g')
            {
                index=ADC_Read(index);
                if(index<=(32*4))//0x20
                    {
                        LATAbits.LATA2 = 0;
                        LATAbits.LATA3 = 0;
                    }
                    else if(index>(32*4)&&index<=(128*4))//0x20 0x80
                    {
                        LATAbits.LATA2 = 0;
                        LATAbits.LATA3 = 1;
                    }
                    else if(index>(128*4)&&index<=(224*4))//0x80 0xE0
                    {
                        LATAbits.LATA2 = 1;
                        LATAbits.LATA3 = 0;
                    }
                    else//0xE0
                    {
                        LATAbits.LATA2 = 1;
                        LATAbits.LATA3 = 1;
                    }
                }
                else if(str[0]=='e') 
                {
                    LATAbits.LATA2 = 0;
                    LATAbits.LATA3 = 0;
                    break;
                }
                ClearBuffer();
            }*/
    }
    
    return ;
}

void Mode3()// Todo : Mode3
{ 
    //Q2b
    ClearBuffer();
    str[0] = 'M';
    str[1] = 'o';
    str[2] = 'd';
    str[3] = 'e';
    str[4] = '3';
    str[5] = '\n';
    str[6] = '\r';
    str[7] = '\0';
    UART_Write_Text(str);
    ClearBuffer();
    
    // Timer2 -> On, prescaler -> 16
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b10;
    
    // Internal Oscillator Frequency, Fosc = 125 kHz, Tosc = 8 탎
    //OSCCONbits.IRCF = 0b001;
    
    // PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.CCP1M = 0b1100;
    
    // CCP1/RC2 -> Output
    TRISC = 0;
    LATC = 0;
    
    // Set up PR2, CCP to decide PWM period and Duty Cycle
    /** 
     * PWM period
     * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
     * = (0xF9 + 1) * 4 * 0.25탎 * 16
     * = 0.004s = 4ms = 4000탎
     */
    PR2 = 0xF9;
    
    /**
     * Duty cycle
     * = (CCPR1L:CCP1CON<5:4>) * Tosc * (TMR2 prescaler)
     * = (0x5A*4 + 0b10) * 0.25탎 *  16
     * = 0.001448s ~= 1450탎 //0
     * 
     * = (0x1F*4 + 0b01) * 0.25탎 * 16
     * = 0.0005s ~= 500탎 //-90
     * 
     * = (0x96*4 + 0b00) * 0.25탎 * 16
     * = 0.0024s ~= 2400탎 //90
     */
    CCPR1L = 0x9D;
    CCP1CONbits.DC1B = 0b10;
    
    count=0;
    duty_cycle = 0x9D;
    duty_low = 0x01;
    clockwise = 0x00;
    index=0;
    
    int degree=90;
    CCPR1L=((degree+90)*126/180)+0x1F;
    
    
    while(1)
    {
        /*strcpy(str, GetString());
        if(str[0]=='m' && str[1]=='o' && str[2]=='d' && str[3]=='e' && str[4]=='1'){ // Mode1
            ClearBuffer();
            str[0] = 'M';
            str[1] = 'o';
            str[2] = 'd';
            str[3] = 'e';
            str[4] = '1';
            str[5] = '\n';
            str[6] = '\r';
            str[7] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            count=1;
        }
        else if(str[0]=='m' && str[1]=='o' && str[2]=='d' && str[3]=='e' && str[4]=='2'){ // Mode2
            ClearBuffer();
            str[0] = 'M';
            str[1] = 'o';
            str[2] = 'd';
            str[3] = 'e';
            str[4] = '2';
            str[5] = '\n';
            str[6] = '\r';
            str[7] = '\0';
            UART_Write_Text(str);
            ClearBuffer();  
            count=2;
        }
        
        if(count==1)
        {
            if(duty_cycle == 0x9D)
            {
                duty_cycle = 0x1F;//500
                CCP1CONbits.DC1B = 0b01;
            }
            else
            {
                duty_cycle = 0x9D;//2400
                CCP1CONbits.DC1B = 0b00;
            }
            CCPR1L = duty_cycle;
            __delay_ms(400);
        }
        else if(count==2)
        {
            if(duty_cycle == 0x9D)
            {
                duty_cycle = 0x5A;//500
                CCP1CONbits.DC1B = 0b01;
            }
            else
            {
                duty_cycle = 0x9D;//2400
                CCP1CONbits.DC1B = 0b00;
            }
            CCPR1L = duty_cycle;
            __delay_ms(400);
        }*/
        
        /*if(count==1)
        {
            if(duty_cycle == 0x9D)
            {
                duty_cycle = 0x1F;//500
                CCP1CONbits.DC1B = 0b01;
            }
            else
            {
                duty_cycle = 0x9D;//2400
                CCP1CONbits.DC1B = 0b00;
            }
            __delay_ms(500);
            CCPR1L = duty_cycle;
        }
        else if(count==2)
        {
            if(clockwise == 0x00)
            {
                duty_cycle = 0xA5;
                duty_low = 0b00;
                clockwise = 0x01;
            }
            else if(clockwise == 0x01)
            {
                if (duty_low == 0x03)
                {
                    duty_low = 0x02;
                }
                else if (duty_low == 0x02)
                {
                    duty_low = 0x01;
                }
                else if (duty_low == 0x01)
                {
                    duty_low = 0x00;
                }
                else
                {
                    duty_cycle -- ;
                    if(duty_cycle == 0x1F)
                    {
                        clockwise = 0x00;
                        duty_low = 0x01;
                    }
                    else
                    {
                        duty_low = 0x03;
                    }
                }
            }
            __delay_ms(10);
            CCPR1L = duty_cycle;
            CCP1CONbits.DC1B = duty_low & 0x03;
        }*/
    }
    
    return ;
}

void Mode4()// Todo : Mode4
{ 
    //PWM+ADC
    ClearBuffer();
    str[0] = 'M';
    str[1] = 'o';
    str[2] = 'd';
    str[3] = 'e';
    str[4] = '4';
    str[5] = '\n';
    str[6] = '\r';
    str[7] = '\0';
    UART_Write_Text(str);
    ClearBuffer();
    
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b01;
    //OSCCONbits.IRCF = 0b001;
    CCP1CONbits.CCP1M = 0b1100;
    
    TRISC = 0;
    LATC = 0;
    
    PR2 = 0xF9;
    CCPR1L = 0x125;
    CCP1CONbits.DC1B = 0b00;
    
    count=0;
    state=100;
    int temp=0;
    
    while(1)
    {
        index=ADC_Read(index);
        index=index>>2;
        
        //Q1b
        /*if(index>=170)
        {
            ClearBuffer();
            str[0] = 'S';
            str[1] = 't';
            str[2] = 'a';
            str[3] = 't';
            str[4] = 'e';
            str[5] = '_';
            str[6] = '1';
            str[7] = ' ';
            str[8] = 'c';
            str[9] = 'o';
            str[10] = 'u';
            str[11] = 'n';
            str[12] = 't';
            str[13] = ' ';
            str[14] = '=';
            str[15] = ' ';
            str[16] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            hundreds=count/100;
            tens=count/10;
            ones=count%10;
            if(hundreds==0)
            {
                if(tens==0)
                {
                    UART_Write(48+ones);
                }
                else
                {
                    UART_Write(48+tens);
                    UART_Write(48+ones);
                }
            }
            else
            {
                UART_Write(48+hundreds);
                UART_Write(48+tens);
                UART_Write(48+ones);
            }
            str[0] = '\r';
            str[1] = '\n';
            str[2] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            if(count%2==0)
            {
                duty_cycle = 0;
            }
            else
            {
                duty_cycle = 125;
            }
            __delay_ms(1000);
            count=count+1;
        }
        else if(index<170 && index>=85)
        {
            ClearBuffer();
            str[0] = 'S';
            str[1] = 't';
            str[2] = 'a';
            str[3] = 't';
            str[4] = 'e';
            str[5] = '_';
            str[6] = '2';
            str[7] = ' ';
            str[8] = 'c';
            str[9] = 'o';
            str[10] = 'u';
            str[11] = 'n';
            str[12] = 't';
            str[13] = ' ';
            str[14] = '=';
            str[15] = ' ';
            str[16] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            hundreds=count/100;
            tens=count/10;
            ones=count%10;
            if(hundreds==0)
            {
                if(tens==0)
                {
                    UART_Write(48+ones);
                }
                else
                {
                    UART_Write(48+tens);
                    UART_Write(48+ones);
                }
            }
            else
            {
                UART_Write(48+hundreds);
                UART_Write(48+tens);
                UART_Write(48+ones);
            }
            str[0] = '\r';
            str[1] = '\n';
            str[2] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            if(count%2==0)
            {
                duty_cycle = 0;
            }
            else
            {
                duty_cycle = 125;
            }
            __delay_ms(500);
            count=count+1;
        }
        else //index>=170
        {
            ClearBuffer();
            str[0] = 'S';
            str[1] = 't';
            str[2] = 'a';
            str[3] = 't';
            str[4] = 'e';
            str[5] = '_';
            str[6] = '3';
            str[7] = ' ';
            str[8] = 'c';
            str[9] = 'o';
            str[10] = 'u';
            str[11] = 'n';
            str[12] = 't';
            str[13] = ' ';
            str[14] = '=';
            str[15] = ' ';
            str[16] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            hundreds=count/100;
            tens=count/10;
            ones=count%10;
            if(hundreds==0)
            {
                if(tens==0)
                {
                    UART_Write(48+ones);
                }
                else
                {
                    UART_Write(48+tens);
                    UART_Write(48+ones);
                }
            }
            else
            {
                UART_Write(48+hundreds);
                UART_Write(48+tens);
                UART_Write(48+ones);
            }
            str[0] = '\r';
            str[1] = '\n';
            str[2] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
            
            if(count%2==0)
            {
                duty_cycle = 0;
            }
            else
            {
                duty_cycle = 125;
            }
            __delay_ms(250);
            count=count+1;
        }*/
        
        
        if(index<35)
        {
            state=0;
        }
        else if(index>=35&&index<70)
        {
            state=1;
        }
        else if(index>=70&&index<105)
        {
            state=2;
        }
        else if(index>=105&&index<140)
        {
            state=3;
        }
        else if(index>=140&&index<175)
        {
            state=4;
        }
        else if(index>=175&&index<210)
        {
            state=5;
        }
        else if(index>=210)
        {
            state=6;
        }
        
        strcpy(str, GetString());
        temp=0;
        for(int i=0;str[i]=='0'||str[i]=='1'||str[i]=='2'||str[i]=='3'||str[i]=='4'||str[i]=='5'||str[i]=='6'||str[i]=='7'||str[i]=='8'||str[i]=='9'||str[i]=='\n'||str[i]=='\r';i++)
        {
            if(str[i]=='\r')
            {
                ClearBuffer();
                str[0] = '\r';
                str[1] = '\n';
                str[2] = '\0';
                UART_Write_Text(str);
                ClearBuffer();
                state=(state+temp)%7;
                break;
            }
            temp=(temp*10)+str[i]-48;
        }
        
        if(state!=count)
        {
            if(state==0)
            {
                count=state;
                LATAbits.LATA1 = 1;
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 1;
                ClearBuffer();
                str[0] = 'L';
                str[1] = 'L';
                str[2] = 'L';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==1)
            {
                count=state;
                LATAbits.LATA1 = 1;
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 0;
                ClearBuffer();
                str[0] = 'L';
                str[1] = 'L';
                str[2] = 'D';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==2)
            {
                count=state;
                LATAbits.LATA1 = 1;
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                ClearBuffer();
                str[0] = 'L';
                str[1] = 'D';
                str[2] = 'D';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==3)
            {
                count=state;
                LATAbits.LATA1 = 0;
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 0;
                ClearBuffer();
                str[0] = 'D';
                str[1] = 'D';
                str[2] = 'D';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==4)
            {
                count=state;
                LATAbits.LATA1 = 0;
                LATAbits.LATA2 = 0;
                LATAbits.LATA3 = 1;
                ClearBuffer();
                str[0] = 'D';
                str[1] = 'D';
                str[2] = 'L';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==5)
            {
                count=state;
                LATAbits.LATA1 = 0;
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 1;
                ClearBuffer();
                str[0] = 'D';
                str[1] = 'L';
                str[2] = 'L';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
            else if(state==6)
            {
                count=state;
                LATAbits.LATA1 = 1;
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 1;
                ClearBuffer();
                str[0] = 'L';
                str[1] = 'L';
                str[2] = 'L';
                str[3] = '\n';
                str[4] = '\r';
                str[5] = '\0';
                UART_Write_Text(str);
                ClearBuffer();  
            }
        }
    }
    
    return ;
}



void main(void)
{
    
    SYSTEM_Initialize() ;
    
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
        
        /*
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        if(str[0]=='m' && str[1]=='1'){ // Mode1
            Mode1();
            ClearBuffer();
            str[0] = 'E';
            str[1] = 'x';
            str[2] = 'i';
            str[3] = 't';
            str[4] = '\n';
            str[5] = '\0';
            UART_Write_Text(str);
            ClearBuffer();
        }
        else if(str[0]=='m' && str[1]=='2'){ // Mode2
            Mode2();
            ClearBuffer();
            str[0] = 'E';
            str[1] = 'x';
            str[2] = 'i';
            str[3] = 't';
            str[4] = '\n';
            str[5] = '\0';
            UART_Write_Text(str);
            ClearBuffer();  
        }
        else if(str[0]=='m' && str[1]=='3'){ // Mode3
            Mode3();
            ClearBuffer();
            str[0] = 'E';
            str[1] = 'x';
            str[2] = 'i';
            str[3] = 't';
            str[4] = '\n';
            str[5] = '\0';
            UART_Write_Text(str);
            ClearBuffer();  
        }
        else if(str[0]=='m' && str[1]=='4'){ // Mode4
            Mode4();
            ClearBuffer();
            str[0] = 'E';
            str[1] = 'x';
            str[2] = 'i';
            str[3] = 't';
            str[4] = '\n';
            str[5] = '\r';
            str[6] = '\0';
            UART_Write_Text(str);
            ClearBuffer();  
        }*/
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{   
    
    if(INTCONbits.INT0IF == 1)
    {
        //ADC
        //LATAbits.LATA2 = 0;
        //LATAbits.LATA3 = 0;
        if(index==0)
        {
            count=1;
            index=1;
        }
        
        if(count==1)//45degree  31
        {
            count=count+1;
            if(clockwise == 0x00)
            {
                if(CCPR1L+31>=0x9D)
                {
                    index=31-0x9D+CCPR1L;
                    CCPR1L=0x9D;
                    
                    clockwise=1;
                    __delay_ms(400);
                    CCPR1L=0x9D-index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L+31;
                    __delay_ms(400);
                }
            }
            else//clockwise == 0x01
            {
                if(CCPR1L-31<=0x1F)
                {
                    index=31-CCPR1L+0x1F;
                    CCPR1L=0x1F;
                    
                    clockwise=0;
                    __delay_ms(400);
                    CCPR1L=0x1F+index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L-31;
                    __delay_ms(400);
                }
            }
        }
        else if(count==2)//90degree  62
        {
            count=count+1;
            if(clockwise == 0x00)
            {
                if(CCPR1L+62>=0x9D)
                {
                    index=62-0x9D+CCPR1L;
                    CCPR1L=0x9D;
                    
                    clockwise=1;
                    __delay_ms(400);
                    CCPR1L=0x9D-index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L+62;
                    __delay_ms(400);
                }
            }
            else//clockwise == 0x01
            {
                if(CCPR1L-62<=0x1F)
                {
                    index=62-CCPR1L+0x1F;
                    CCPR1L=0x1F;
                    
                    clockwise=0;
                    __delay_ms(400);
                    CCPR1L=0x1F+index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L-62;
                    __delay_ms(400);
                }
            }
        }
        else if(count==3)//180degree  124
        {
            count=1;
            if(clockwise == 0x00)
            {
                if(CCPR1L+124>=0x9D)
                {
                    index=124-0x9D+CCPR1L;
                    CCPR1L=0x9D;
                    clockwise=1;
                    __delay_ms(400);
                    CCPR1L=0x9D-index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L+124;
                    __delay_ms(400);
                }
            }
            else//clockwise == 0x01
            {
                if(CCPR1L-124<=0x1F)
                {
                    index=124-CCPR1L+0x1F;
                    CCPR1L=0x1F;
                    
                    clockwise=0;
                    __delay_ms(400);
                    CCPR1L=0x1F+index;
                    __delay_ms(400);
                }
                else
                {
                    CCPR1L=CCPR1L-124;
                    __delay_ms(400);
                }
            }
        }
        INTCONbits.INT0IF = 0;
        //ClearBuffer();
        //__delay_ms(500);
    }
    
    /*if(INTCONbits.INT0IF == 1)
    {
        while(clockwise == 0x00)
        {
            if (duty_low == 0x00)
            {
                duty_low = 0x01;
            }
            else if (duty_low == 0x01)
            {
                duty_low = 0x02;
            }
            else if (duty_low == 0x02)
            {
                duty_low = 0x03;
            }
            else
            {
                duty_cycle ++ ;
                if(duty_cycle == 0x13)
                {
                    clockwise = 0x01;
                }
                else
                {
                    duty_low = 0x00;
                }
            }
            __delay_ms(20);
            CCPR1L = duty_cycle;
            CCP1CONbits.DC1B = duty_low & 0x03;
        }
        __delay_ms(40);
        while(clockwise == 0x01)
        {
            if (duty_low == 0x03)
            {
                duty_low = 0x02;
            }
            else if (duty_low == 0x02)
            {
                duty_low = 0x01;
            }
            else if (duty_low == 0x01)
            {
                duty_low = 0x00;
            }
            else
            {
                duty_cycle -- ;
                if(duty_cycle == 0x04)
                {
                    clockwise = 0x00;
                }
                else
                {
                    duty_low = 0x03;
                }
            }
            __delay_ms(20);
            CCPR1L = duty_cycle;
            CCP1CONbits.DC1B = duty_low & 0x03;
        }
    }
    INTCONbits.INT0IF = 0;*/
    
    return;
    
}