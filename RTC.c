#include "headers.h"

// Starts read request for RTC
void reqReadRTC(unsigned char address) {
    I2C_Start();
    I2C_WriteByte(DS1307w); // RTC slave address + write bit
    I2C_WriteByte(address); // address to begin reading from
    I2C_ReStart();          // Second start to let RTC know it is a read request
    I2C_WriteByte(DS1307r); // RTC slave address + read bit
                            // RTC ready to transmit data
}

// Reads data from RTC
unsigned char readDataRTC(void) {
    unsigned char data;
    data = I2C_ReadByte(); // stores read byte
    return data;
}

// Ends read from RTC
void endReadRTC(void) {
    I2C_SendNACK(); // Sends not acknowledge to signal end of read
    I2C_Stop();     // Sends stop bit
}

// Reads only one byte from RTC (created separate functions for ease of looping)
unsigned char readByteRTC(unsigned char address) {
    unsigned char data = 0x00;
    reqReadRTC(address);   // Requests read from address location
    data = I2C_ReadByte(); // Stores read byte into data
    endReadRTC();          // Ends read
    return data;
}

// Sends request to begin writing to slave
void reqWriteRTC(unsigned char address) {
    I2C_Start();
    I2C_WriteByte(DS1307w); // Slave address + write bit
    I2C_WriteByte(address); // address to start writing in
}

// Sends byte to slave
void writeDataRTC(unsigned char data) {
    I2C_WriteByte(data); // Sends byte to slave
}

// Ends write request
void endWriteRTC(void) {
    I2C_Stop();
}

// Writes one byte to RTC (created separate functions for ease of looping)
void writeByteRTC(unsigned char address, unsigned char data) {
    reqWriteRTC(address); // Requests to begin writing to specified address
    writeDataRTC(data);   // Byte to write
    endWriteRTC();        // Ends write request
}

// Checks to see if RTC is working
__bit isRTCRunning(void) {
    unsigned char data;
    reqReadRTC(0x00);       // Reads address 00h (interested in CH which is bit 7)
    data = I2C_ReadByte();
    return BIT_CHECK(data,7); // returns 1 if CH is set(clock is not started) and 0 if CH
}

// Starts RTC and initializes starting date and time
void startRTC(void) {
    reqWriteRTC(0x00);  // writes to address 00h 
    writeDataRTC(0x00); // clears the seconds register and starts a halted DS1307 (doesn't hurt a DS3232)
    //Fill clock with July 7, 2010, 12:30PM (first D-mail was sent), otherwise clock starts in 01/01/00 00:00:00
    writeDataRTC(0x30); // 30 minutes
    writeDataRTC(0x12); // 12 hours
    writeDataRTC(0x28); // 28th
    writeDataRTC(0x7);  // 7 (July)
    writeDataRTC(0x10); // 10 (2010)
    endWriteRTC();
    //Clears clock RAM for tube blanking hours (because they are garbage currently)
    reqWriteRTC(0x14);   // blankStart RAM address
    writeDataRTC(0x00);  // writes 00000000 to clear address
    writeDataRTC(0x00);  // writes 00000000 to 0x15 (blankEnd RAM address) to clear it      
    endWriteRTC();
}

// Checks to see if RTC is a running DS1307 or a DS3232 (running or stopped)
__bit checkRTCType(void) {
    unsigned char data;
    data = readByteRTC(0x0F);         // Memory address for Oscillator Stopped Flag (OSF)on DS3232; RAM register on DS1307
    if(BIT_CHECK(data,7)) {           // If OSF is clear, Clock is running (with battery backup)
                                      // Otherwise this is a stopped DS3232 or the DS1307 RAM at 0Fh bit 7 happened to be set
        // Check to see if it is a stopped DS3232 or just a set bit in the DS1307
        BIT_CLEAR(data,0);            // writes zero to bit 0 just in case there was a 1 there from the start
        writeByteRTC(0x0F,data); 
        BIT_SET(data,0);              // writes one to bit 0
        writeByteRTC(0x0F,data); 
        __delay_us(5);                // Delay by 5 usec (honestly just put this here just in case prob not needed will test)
        data = readByteRTC(0x0F);     // Read back from 0Fh
        if(!BIT_CHECK(data,0)) {      // If the bit is not set it is a stopped DS3232. Bit cannot be 1 in a DS3232
            writeByteRTC(0x0F, 0x00); // Clears the 0Fh register to start the DS3232 Clock
            return 1;                 // Returns a one to start the clock/fill the clock
        }
    } 
    return 0;                         // If OSF is clear or clock is DS1307 return 0 (if any clock is working);
}

void getTime(void) {
    unsigned char seconds, minutes, hours;
    unsigned char singleSeconds, singleMinutes, singleHours;
    unsigned char tensSeconds, tensMinutes, tensHours;
    reqReadRTC(0x00); // Seconds address
    seconds = readDataRTC(); // Reads seconds and 
    I2C_SendACK();           // sends ACK to increment to minutes address
    minutes = readDataRTC(); // Reads minutes and 
    I2C_SendACK();           // sends ACK to increment to hours address
    hours = readDataRTC();   // Reads minutes
    endReadRTC();
    singleSeconds = (seconds & 0x0F); // Keeps only ones digit of seconds
    tensSeconds = (swapNibbles(seconds) & 0x0F); // Swaps nibbles to get tens digit of seconds
    singleMinutes = (minutes & 0x0F); // Keeps only ones digit of minutes
    tensMinutes = (swapNibbles(minutes) & 0x0F); // Swaps nibbles to get tens digit of minutes
    singleHours = (hours & 0x0F); // Keeps only ones digit of hours
    tensHours = (swapNibbles(hours) & 0x0F); // Swaps nibbles to get tens digit of hours
    passTubeNum(tensHours,singleHours,10,tensMinutes,singleMinutes,10,tensSeconds,singleSeconds,0x00,0x24); //Puts the time into tubes, 0x24 == 0b00100100 enables right decimal point on blank tubes
}