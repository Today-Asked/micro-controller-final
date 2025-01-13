#include <xc.h>
#include <stdint.h>
#include <math.h>
#include <pic18f4520.h>
#include "gyroscope.h"
#include "uart.h"

#define LEFT -1
#define RIGHT 1
#define OFF 0

#define _XTAL_FREQ 4000000
#define MPU_ADDR 0x68

#define ACCX_OFFSET 0
#define ACCZ_OFFSET 0

#define LED LATA

float pitch_offset = 0.0;

int16_t accX, accZ;
uint8_t accX_high, accX_low, accZ_high, accZ_low;
float angle;

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
    accZ_high = I2C_Read(1);
    accZ_low = I2C_Read(0);
    I2C_Stop();

    
    accX = (accX_high << 8) | accX_low;
    accZ = (accZ_high << 8) | accZ_low;

    
    angle = calculate_angle(accX, accZ) - pitch_offset;

    
    if (angle > 45.0) {
        UART_Write_Text("LLN\r\n");
    } else if (angle < -45.0) {
        UART_Write_Text("LRN\r\n");
    } else {
        
    }

    __delay_ms(100);
}

float calculate_angle(int16_t accX, int16_t accZ) {
    return atan2((float)(accX - ACCX_OFFSET), (float)(accZ - ACCZ_OFFSET)) * 180.0 / M_PI;
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
    accZ_high = I2C_Read(1);
    accZ_low = I2C_Read(0);
    I2C_Stop();

    accX = (accX_high << 8) | accX_low;
    accZ = (accZ_high << 8) | accZ_low;
    pitch_offset =  atan2((float)accX, (float)accZ) * 180.0 / M_PI;
}
