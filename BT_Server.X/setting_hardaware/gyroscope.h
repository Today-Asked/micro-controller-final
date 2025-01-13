#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H

#include "xc.h"

void Gyroscope_Initialize(void);
void Check_Gyroscope(void);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);
void Calibration(void);
float calculate_angle(int16_t accX, int16_t accZ);