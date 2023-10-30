#ifndef IIC_H
#define	IIC_H

//PIC is using internal oscillator
#define _XTAL_FREQ 4000000          //Running at 4MHz

// Define I2C pins
#define SCL PORTAbits.RA0           //SCL clock line on RA0 for I2C
#define SDA PORTAbits.RA1           //SDA data line on RA1 for I2C
#define SCL_IO TRISAbits.TRISA0     // Clock pin input/output
#define SDA_IO TRISAbits.TRISA1     // Data pin input/output

// Define I2C speed
#define I2C_SPEED 100               //kbps (maximum DS1307 supports, and because we want to maximize THOUGHPUT)
#define HalfBitDelay 500/I2C_SPEED  //usec

// Function declarations
void InitI2C(void);
void I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
void I2C_WriteByte(unsigned char Data);
unsigned char I2C_ReadByte(void);
void I2C_SendACK(void);
void I2C_SendNACK(void);

#endif	/* IIC_H */

