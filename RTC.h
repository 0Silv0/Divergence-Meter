#ifndef RTC_H
#define	RTC_H

// DS1307/DS3232 Address
#define DS1307w 0b11010000 // Slave address for DS1307 + write bit
#define DS1307r 0b11010001 // Slave address for DS1307 + read bit

// Function Declarations
void reqReadRTC(unsigned char address);
unsigned char readDataRTC(void);
void endReadRTC(void);
unsigned char readByteRTC(unsigned char address);
void reqWriteRTC(unsigned char address);
void writeDataRTC(unsigned char data);
void endWriteRTC(void);
void writeByteRTC(unsigned char address, unsigned char data);
__bit isRTCRunning(void);
void startRTC(void);
__bit checkRTCType(void);
void getTime(void);
#endif	/* RTC_H */

