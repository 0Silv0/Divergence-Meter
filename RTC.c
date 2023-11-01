#include "headers.h"

unsigned char oldSeconds;

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
    data = readByteRTC(0x00);       // Reads address 00h (interested in CH which is bit 7)
    return BIT_CHECK(data,7); // returns 1 if CH is set(clock is not started) and 0 if CH is clear
}

// Starts RTC and initializes starting date and time
void startRTC(void) {
    unsigned char clockTest, RTCaddress;
    reqWriteRTC(0x00);  // writes to address 00h 
    writeDataRTC(0x00); // clears the seconds register and starts a halted DS1307 (doesn't hurt a DS3232)
    //Fill clock with July 7, 2010, 12:30PM (first D-mail was sent), otherwise clock starts in 01/01/00 00:00:00
    writeDataRTC(0x30); // 30 minutes
    writeDataRTC(0x12); // 12 hours
    endWriteRTC();
    reqWriteRTC(0x04);
    writeDataRTC(0x28); // 28th
    writeDataRTC(0x7);  // 7 (July)
    writeDataRTC(0x10); // 10 (2010)
    endWriteRTC();
    //Clears clock RAM for tube blanking hours (because they are garbage currently)
    reqWriteRTC(0x14);   // blankStart RAM address
    writeDataRTC(0x00);  // writes 00000000 to clear address
    writeDataRTC(0x00);  // writes 00000000 to 0x15 (blankEnd RAM address) to clear it      
    endWriteRTC();
    
    if(BIT_CHECK(Flag, typeRTC)) {
        RTCaddress = 0x00; // Address to check in DS1307
    } else {
        RTCaddress = 0x0F; // Address to check in DS3232
    }
    // Checks to see if it started properly
    clockTest = readByteRTC(RTCaddress);
    if(!BIT_CHECK(clockTest,7)) {
        BIT_CLEAR(ErrFlag,ErrRTC);  // Clears error flag if RTC started
        BIT_CLEAR(ErrFlag,ErrNACK); // Clears NACK since obviously RTC acknowledged 
    } else {
        // Throws error flag if not started properly
        BIT_SET(ErrFlag,ErrRTC);
        displayError();
    }
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
            BIT_CLEAR(Flag, typeRTC); // Sets RTC type flag to DS3232
            return 1;                 // Returns a one to start the clock/fill the clock
        } else {
            BIT_SET(Flag, typeRTC);   // Sets RTC type flag to DS1307
        }
    } 
    return 0;                         // If OSF is clear or clock is DS1307 return 0 (if any clock is working);
}

void getTime(void) {
    unsigned char seconds, minutes, hours;
    unsigned char singleSeconds, singleMinutes, singleHours;
    unsigned char tensSeconds, tensMinutes, tensHours;
    unsigned char tmpLeft, tmpRight;
    
    seconds = readByteRTC(0x00); // Reads only seconds from RTC to check if update needs to be performed
    // Checks to see if RTC is stopped for some reason
    if(BIT_CHECK(seconds,7)) {
        BIT_SET(ErrFlag, ErrRTC);
    } else {
        // Only updates display if its a new second, to avoid unnecessary instruction cycles
        if(seconds != oldSeconds) {
            if(seconds == 0b00110000) { // 30 in BCD
                getDate(); // Displays current date
                __delay_ms(4000); // Delay four seconds for date display
            } else if (seconds == 0b01011001) { // 59 in BCD
                //do animation(); but that doesn't exist yet 
            } else {
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
                // Swaps decimal points between left and right depending if its a new second. Even numbers in binary end in 0 and odd end in 1.
                if(BIT_CHECK(seconds,0)) {
                    tmpLeft = 0x00;  // disables decimal points in all tubes
                    tmpRight = 0x24; // 0x24 == 0b00100100 enables decimal point on tubes 2 and 4
                } else {
                    tmpLeft = 0x24;
                    tmpRight = 0x00;
                }
                passTubeNum(tensHours,singleHours,10,tensMinutes,singleMinutes,10,tensSeconds,singleSeconds,tmpLeft,tmpRight); //Puts the time into tubes
                oldSeconds = seconds;                
            }
        }
    }
}

void getDate(void) {
    unsigned char day, month, year;
    unsigned char singleDay, singleMonth, singleYear;
    unsigned char tensDay, tensMonth, tensYear;
    reqReadRTC(0x04);
    day = readDataRTC();
    I2C_SendACK();
    month = readDataRTC();
    I2C_SendACK();
    year = readDataRTC();
    endReadRTC();
    singleDay = (day & 0x0F); // Keeps only ones digit of seconds
    tensDay = (swapNibbles(day) & 0x0F); // Swaps nibbles to get tens digit of seconds
    singleMonth = (month & 0x0F); // Keeps only ones digit of minutes
    tensMonth = (swapNibbles(month) & 0x0F); // Swaps nibbles to get tens digit of minutes
    singleYear = (year & 0x0F); // Keeps only ones digit of hours
    tensYear = (swapNibbles(year) & 0x0F); // Swaps nibbles to get tens digit of hours       
    passTubeNum(tensDay,singleDay,10,tensMonth,singleMonth,10,tensYear,singleYear,0x00,0x00);
}