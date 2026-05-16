/**
 * @file i2c.h
 * @author silv
 * @date 2026-05-13
 * @brief I2C function declarations
 */

// i2c.h — I2C Master Library for PIC16F1846
#ifndef I2C_H
#define I2C_H

#include <xc.h>
#include <stdint.h>

void I2C_Init(void);
uint8_t I2C_Write(uint8_t addr, const uint8_t *data, uint8_t len);
uint8_t I2C_Read(uint8_t addr, uint8_t *buf, uint8_t len);
uint8_t I2C_ReadRegister(uint8_t address, uint8_t registerAddress, uint8_t *buffer, uint8_t numberOfBytes);
uint8_t I2C_WriteRegister(uint8_t address, uint8_t registerAddress, const uint8_t *buffer, uint8_t numberOfBytes);

#endif /* I2C_H */