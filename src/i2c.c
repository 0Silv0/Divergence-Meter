/**
 * @file i2c.c
 * @author silv
 * @date 2026-05-13
 * @brief I2C Master Library for PIC16F1846 function implementations | Blocking/polling, uses hardware MSSP module (SSP1) | SDA = RB1 (pin 7), SCL = RB4 (pin 10)
 */

#include "../inc/i2c.h"
#include "../inc/vars.h"

// * I2C local helper functions *

// Wait for the MSSP to finish its current operation (SSPIF set)
// Returns I2C_OK or I2C_TIMEOUT.
static uint8_t i2c_wait(void) {
    uint16_t timeout = 10000;
    while (!SSP1IF) {
        if (--timeout == 0)
            return I2C_TIMEOUT;
    }
    SSP1IF = 0;
    return I2C_OK;
}

// Wait for the bus to become idle (no Start, Stop, or data in progress)
static uint8_t i2c_wait_idle(void) {
    uint16_t timeout = 10000;
    // ACKEN, RCEN, PEN, RSEN, SEN must all be clear; also check BF
    while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04)) {
        if (--timeout == 0)
            return I2C_TIMEOUT;
    }
    return I2C_OK;
}

// Send start condition
static uint8_t i2c_start(void) {
    uint8_t busCondition;
    // If bus is busy, return timeout error
    if ((busCondition = i2c_wait_idle()) != I2C_OK)
        return busCondition;
    SEN = 1; // Otherwise, initiate I2C Start
    return i2c_wait();
}

// Send a repeated start condition
static uint8_t i2c_repeated_start(void) {
    uint8_t busCondition;
    // If bus is busy, return timeout error
    if ((busCondition = i2c_wait_idle()) != I2C_OK)
        return busCondition;
    RSEN = 1; // Otherwise, initiate I2C repeated start
    return i2c_wait();
}

// Send stop condition
static uint8_t i2c_stop(void) {
    uint8_t busCondition;
    // If bus is busy, return timeout error
    if ((busCondition = i2c_wait_idle()) != I2C_OK)
        return busCondition;
    PEN = 1; // Otherwise, initiate Stop
    return i2c_wait();
}

// Write one byte; returns I2C_OK, I2C_NACK, or I2C_TIMEOUT
static uint8_t i2c_write_byte(uint8_t data) {
    uint8_t busCondition;
    // If bus is busy, return timeout error
    if ((busCondition = i2c_wait_idle()) != I2C_OK)
        return busCondition;
    SSP1BUF = data; // Otherwise, write data to buffer and let MSSP handle transmission
    // If MSSP doesn't finish in time, return timeout error
    if ((busCondition = i2c_wait()) != I2C_OK)
        return busCondition;
    if (ACKSTAT) // If NACK is recieved, return NACK error
        return I2C_NACK;
    return I2C_OK;
}

// Read one byte; set ackNack to 0 to ACK (more bytes to read), or 1 to NACK (last byte to read)
static uint8_t i2c_read_byte(uint8_t *data, uint8_t ackNack) {
    uint8_t busCondition;
    // If bus is busy, return timeout error
    if ((busCondition = i2c_wait_idle()) != I2C_OK)
        return busCondition;
    RCEN = 1; // Otherwise, begin to receive data
    // If MSSP doesn't finish in time, return timeout error
    if ((busCondition = i2c_wait()) != I2C_OK)
        return busCondition;
    *data = SSP1BUF; // Read received byte from buffer

    // Send ACK or NACK
    ACKDT = ackNack; // 1 = NACK, 0 = ACK
    ACKEN = 1; // Initiate acknowledge sequence
    return i2c_wait();
}

// * I2C API functions *

// Initilize MSSP in I2C master mode with the specified clock speed
void I2C_Init(void) {

    // Configure SDA and SCL as inputs — the MSSP drives them open-drain
    SDA = 1;   // SDA input
    SCL = 1;   // SCL input

    SSP1CON1 = 0x00;        // Disable MSSP first
    SSP1CON2 = 0x00;        // Clear all master control bits
    SSP1CON3 = 0x00;        // Clear stretch/hold bits
    SSP1STAT = 0x80;        // SMP=1, CKE=0

    SSP1ADD  = I2C_SSPADD;  // Sets baud rate
    SSP1CON1 = 0x28;        // SSPEN=1, SSPM=1000 (I2C master)
    PIR1bits.SSP1IF = 0;    // Clear interrupt flag
}

// I2C Write: Write 'numberOfBytes' bytes from 'data' to device at 'address'
uint8_t I2C_Write(uint8_t address, const uint8_t *data, uint8_t numberOfBytes) {

    uint8_t busCondition;
    uint8_t i;

    // If i2c_start() fails, return the error code
    if ((busCondition = i2c_start()) != I2C_OK)
        return busCondition;

    // Sends address with W bit (bit 0 = 0); if it fails, stop and return the error code
    if ((busCondition = i2c_write_byte((address << 1) | 0x00)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    // Write data bytes; if any fail, stop and return the error code
    for (i = 0; i < numberOfBytes; i++) {
        if ((busCondition = i2c_write_byte(data[i])) != I2C_OK) {
            i2c_stop();
            return busCondition;
        }
    }

    // Sends stop condition if all bytes were written successfully, and return the result (I2C_OK or I2C_TIMEOUT)
    return i2c_stop();
}

uint8_t I2C_Read(uint8_t address, uint8_t *buffer, uint8_t numberOfBytes) {

    uint8_t busCondition;
    uint8_t i, is_last;

    // If i2c_start() fails, return the error code
    if ((busCondition = i2c_start()) != I2C_OK)
        return busCondition;

    // Send address with R bit (bit 0 = 1); if it fails, stop and return the error code
    if ((busCondition = i2c_write_byte((address << 1) | 0x01)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    // Read data bytes; ACK all but the last byte, which should be NACKed to signal end of read. If any read fails, stop and return the error code
    for (i = 0; i < numberOfBytes; i++) {
        // NACK the last byte to signal end of read
        is_last = (i == numberOfBytes - 1);
        // If i2c_read_byte() fails, stop and return the error code
        if ((busCondition = i2c_read_byte(&buffer[i], is_last)) != I2C_OK) {
            i2c_stop();
            return busCondition;
        }
    }

    // Sends stop condition if all bytes were read successfully, and return the result (I2C_OK or I2C_TIMEOUT)
    return i2c_stop();
}

// Read 'numberOfBytes' bytes from device at 'address', starting at 'registerAddress' address. Returns I2C_OK, I2C_NACK, or I2C_TIMEOUT.
uint8_t I2C_ReadRegister(uint8_t address, uint8_t registerAddress, uint8_t *buffer, uint8_t numberOfBytes) {

    uint8_t busCondition;
    uint8_t i, is_last;

    // Write phase: send device address + register pointer
    if ((busCondition = i2c_start()) != I2C_OK) 
        return busCondition;

    if ((busCondition = i2c_write_byte((address << 1) | 0x00)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    if ((busCondition = i2c_write_byte(registerAddress)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    // Repeated Start, then read phase
    if ((busCondition = i2c_repeated_start()) != I2C_OK) 
        return busCondition;

    if ((busCondition = i2c_write_byte((address << 1) | 0x01)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    // Read data bytes; ACK all but the last byte, which should be NACKed to signal end of read. If any read fails, stop and return the error code
    for (i = 0; i < numberOfBytes; i++)
    {
        is_last = (i == numberOfBytes - 1);
        if ((busCondition = i2c_read_byte(&buffer[i], is_last)) != I2C_OK)
        {
            i2c_stop();
            return busCondition;
        }
    }

    return i2c_stop();
}

// Write 'numberOfBytes' bytes from 'buffer' to device at 'address', starting at 'registerAddress' address. Returns I2C_OK, I2C_NACK, or I2C_TIMEOUT.
uint8_t I2C_WriteRegister(uint8_t address, uint8_t registerAddress, const uint8_t *buffer, uint8_t numberOfBytes) {

    uint8_t busCondition;
    uint8_t i;

    if ((busCondition = i2c_start()) != I2C_OK)
        return busCondition;

    if ((busCondition = i2c_write_byte((address << 1) | 0x00)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    if ((busCondition = i2c_write_byte(registerAddress)) != I2C_OK) {
        i2c_stop();
        return busCondition;
    }

    for (i = 0; i < numberOfBytes; i++) {
        if ((busCondition = i2c_write_byte(buffer[i])) != I2C_OK) {
            i2c_stop();
            return busCondition;
        }
    }

    return i2c_stop();
}