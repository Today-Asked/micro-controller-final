#include <xc.h>
#include <stdint.h>
#include <math.h>
#include <pic18f4520.h>
#include "gyroscope.h"
#include "uart.h"

#define blink_left() LATAbits.LATA6 = 1;
#define blink_right() LATAbits.LATA5 = 1;
#define stop_blinking() LATAbits.LATA5 = 0; LATAbits.LATA6 = 0;
#define brake_on() LATAbits.LATA7 = 1;
#define brake_off() LATAbits.LATA7 = 0;

#define _XTAL_FREQ 4000000
#define MPU_ADDR 0x68

#define ANGLE_THRESHOLD 30.0

#define ACCX_OFFSET 0
#define ACCZ_OFFSET 0

#define LED LATA

float pitch_offset = 0.0;

int16_t accX, accY, accZ;
uint8_t accX_high, accX_low, accY_high, accY_low, accZ_high, accZ_low;
float angle;

int dir;
int from_mid = 0;
int postpostscaler;

void Gyroscope_Initialize(void) {
    OSCCON = 0x60;
    
    I2C_Init();
    __delay_ms(100);

    
    I2C_Start();
    I2C_Write(MPU_ADDR << 1);
    I2C_Write(0x6B);
    I2C_Write(0x00);
    I2C_Stop();

    Calibration();
}

void Check_Gyroscope(int blinker_dir){
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

    
    if (angle > ANGLE_THRESHOLD) { // left
        if (dir == -1 && from_mid) {
            dir = 0;
            from_mid = 0;
            stop_blinking();
        } else if (dir == 0 && from_mid) {
            dir = -1;
            from_mid = 0;
            blink_left();
        }
        __delay_ms(300);
    } else if (angle < (-1) * ANGLE_THRESHOLD) { // right
        if (dir == 1 && from_mid) {
            dir = 0;
            from_mid = 0;
            stop_blinking();
        } else if (dir == 0 && from_mid) {
            dir = 1;
            from_mid = 0;
            blink_right();
        }
        __delay_ms(300);
    } else {
        from_mid = 1;
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
    int16_t accX, accZ;
    uint8_t accX_high, accX_low, accZ_high, accZ_low;
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