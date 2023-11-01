#include "headers.h"

// Sets initial values of SDA and SCL pins
void InitI2C(void) {
    SCL_IO = 0;
    SDA_IO = 0;
    // Writes zero in output register of SDA and SCL pins
    SDA = 0;
    SCL = 0;
}

// Sends start bit sequence
void I2C_Start(void) {
    SCL_IO = 1;                    // SCL set to high
    SDA_IO = 1;                    // SDA set to high
    __delay_us(HalfBitDelay);   //Half bit delay
    SDA_IO = 0;                    // SDA set to low
    __delay_us(HalfBitDelay);   //Half bit delay
}

// Sends second start bit sequence
void I2C_ReStart(void) {
    SCL_IO = 0;
    __delay_us(HalfBitDelay/2);
    SDA_IO = 1;
    __delay_us(HalfBitDelay/2);
    SCL_IO = 1;
    __delay_us(HalfBitDelay/2);
    SDA_IO = 0;
    __delay_us(HalfBitDelay/2);
}

// Sends stop bit sequence
void I2C_Stop(void) {
    SCL_IO = 0;
    __delay_us(HalfBitDelay/2);
    SDA_IO = 0;
    __delay_us(HalfBitDelay/2); 
    SCL_IO = 1;
    __delay_us(HalfBitDelay/2); 
    SDA_IO = 1;
    __delay_us(HalfBitDelay/2);
}
// I2C data write from master to slave. 
void I2C_WriteByte(unsigned char Data) {
    unsigned char i;
    for(i = 0; i<8; i++) {
        SCL_IO = 0;
        __delay_us(HalfBitDelay/2);
        SDA_IO = ((Data<<i)&0x80)?1:0;
        __delay_us(HalfBitDelay/2);
        SCL_IO = 1;   
        __delay_us(HalfBitDelay);
    }
    
    // Get ACK from slave
    SCL_IO = 0;
    SDA_IO = 1;
    __delay_us(HalfBitDelay);
    SCL_IO = 1;
    __delay_us(HalfBitDelay);
    
    // Waits for RTC to respond
    unsigned char timer = 255;
    while(timer != 0) {
        if(!SDA) {
            // RTC responded, exit wait loop
            timer = 0;
        } else {
            __delay_us(1);
            timer--;
        }
    }
    // If timer ran out RTC is not communicating correctly, throw error flag
    if(timer == 0 && SDA) {
        BIT_SET(ErrFlag, ErrNACK);
    }
    // Returns 0 for ACK and 1 for NACK.
}

// Reads 8 bits and returns it as a byte
unsigned char I2C_ReadByte(void) {
    unsigned char i, Data = 0;
    for(i=0; i<8; i++) {
        SCL_IO = 0;
        SDA_IO = 1;
        __delay_us(HalfBitDelay);
        SCL_IO = 1;
        __delay_us(HalfBitDelay/2);
        Data = Data|(SDA<<(7-i));
        __delay_us(HalfBitDelay/2);
    }
    return Data;
}

//Send ACK to slave
void I2C_SendACK(void) {
    SCL_IO = 0;
    __delay_us(HalfBitDelay/2);
    SDA_IO = 0;
    __delay_us(HalfBitDelay/2);
    SCL_IO = 1;
    __delay_us(HalfBitDelay);
}

//Send NACK to slave
void I2C_SendNACK(void) {
    SCL_IO = 0;
    __delay_us(HalfBitDelay/2);
    SDA_IO = 1;
    __delay_us(HalfBitDelay/2);
    SCL_IO = 1;
    __delay_us(HalfBitDelay);
}


