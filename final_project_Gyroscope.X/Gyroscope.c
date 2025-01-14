#include <xc.h>
#include <stdint.h>
#include <math.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
#pragma config BOREN = ON    // Brown-out Reset Enable bit
#pragma config PBADEN = OFF  // Watchdog Timer Enable bit
#pragma config LVP = OFF     // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF     // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)


#define _XTAL_FREQ 4000000
#define MPU_ADDR 0x68

#define ACCX_OFFSET 0
#define ACCZ_OFFSET 0

#define LED LATA

float pitch_offset = 0.0;

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);
void Calibration(void);
float calculate_angle(int16_t accX, int16_t accY, int16_t accZ);


void main(void) {
    OSCCON = 0x60;
    int16_t accX, accY, accZ;
    uint8_t accX_high, accX_low, accY_high, accY_low, accZ_high, accZ_low;
    float angle;
    ADCON1 = 0x0E;
    TRISA = 0x01;
    
    I2C_Init();
    __delay_ms(100);

    
    I2C_Start();
    I2C_Write(MPU_ADDR << 1);
    I2C_Write(0x6B);
    I2C_Write(0x00);
    I2C_Stop();

    
    TRISA = 0x00;                             
    LATA = 0x00;
    Calibration();
    
    
    while (1) {
        
        I2C_Start();
        I2C_Write(MPU_ADDR << 1);
        I2C_Write(0x3B);
        I2C_Start();
        I2C_Write((MPU_ADDR << 1) | 1);

        accX_high = I2C_Read(1);
        accX_low = I2C_Read(1);
        accY_high = I2C_Read(1);
        accY_low = I2C_Read(1);
        accZ_high = I2C_Read(1);
        accZ_low = I2C_Read(0);
        I2C_Stop();

        
        accX = (accX_high << 8) | accX_low;
        accY = (accY_high << 8) | accY_low;
        accZ = (accZ_high << 8) | accZ_low;

        
        angle = calculate_angle(accX, accY, accZ) - pitch_offset;

        
        if (angle > 45.0) {
            LED = 0x02;
        } else if (angle < -30.0) {
            LED = 0x04;
        } else {
            LED = 0x00;
        }

        __delay_ms(100);
    }
}


float calculate_angle(int16_t accX, int16_t accY, int16_t accZ) {
    return atan2((float)accX, sqrt((float)accY * accY + (float)accZ * accZ)) * 180.0 / M_PI;
}


void I2C_Init(void) {
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;

    SSPADD = 9; 
    SSPSTAT = 0x00;
}


void I2C_Start(void) {
    SEN = 1;
    while(SEN);
}


void I2C_Stop(void) {
    PEN = 1;
    while(PEN);
}

void I2C_Write(uint8_t data) {
    SSPBUF = data;
    while(BF);
    while(SSPCON2bits.ACKSTAT);
}


uint8_t I2C_Read(uint8_t ack) {
    uint8_t data;
    RCEN = 1;
    while(!BF);
    data = SSPBUF;


    ACKDT = ack ? 0 : 1;
    ACKEN = 1; 
    while(ACKEN);
    return data;
}

void Calibration(void){
    int16_t accX, accY, accZ;
    uint8_t accX_high, accX_low, accY_high, accY_low, accZ_high, accZ_low;
    I2C_Start();
    I2C_Write(MPU_ADDR << 1);
    I2C_Write(0x3B);
    I2C_Start();
    I2C_Write((MPU_ADDR << 1) | 1);
    accX_high = I2C_Read(1);
    accX_low = I2C_Read(1);
    accY_high = I2C_Read(1);
    accY_low = I2C_Read(1);
    accZ_high = I2C_Read(1);
    accZ_low = I2C_Read(0);
    I2C_Stop();

    accX = (accX_high << 8) | accX_low;
    accY = (accY_high << 8) | accY_low;
    accZ = (accZ_high << 8) | accZ_low;
    pitch_offset =  calculate_angle(accX, accY, accZ);
}

