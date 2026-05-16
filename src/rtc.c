/**
 * @file rtc.c
 * @author silv
 * @date 2026-05-16
 * @brief RTC function implementations
 */

#include "../inc/rtc.h"

static uint8_t oldSeconds;

// Checks to see if RTC is working
__bit isRTCHalted(void) {
    uint8_t data = readByteRTC(0x00); // Reads address 00h (interested in CH which is bit 7)
    return BIT_CHECK(data,7); // returns 1 if CH is set(clock is not started) and 0 if CH is clear
}

// Starts RTC and initializes starting date and time
void startRTC(void) {
    uint8_t clockTest, RTCaddress;
    reqWriteRTC(0x00);    // writes to address 00h 
    I2C_WriteByte(0x00);  // clears the seconds register and starts a halted DS1307 (doesn't hurt a DS3232)
    //Fill clock with July 7, 2010, 12:30PM (first D-mail was sent), otherwise clock starts in 01/01/00 00:00:00
    I2C_WriteByte(0x30);  // 30 minutes
    I2C_WriteByte(0x12);  // 12 hours
    I2C_Stop();
    reqWriteRTC(0x04);
    I2C_WriteByte(0x28);  // 28th
    I2C_WriteByte(0x7);   // 7 (July)
    I2C_WriteByte(0x10);  // 10 (2010)
    I2C_Stop();
    //Clears clock RAM for tube blanking hours (because they are garbage currently)
    reqWriteRTC(0x14);    // blankStart RAM address
    I2C_WriteByte(0x00);  // writes 00000000 to clear address
    I2C_WriteByte(0x00);  // writes 00000000 to 0x15 (blankEnd RAM address) to clear it      
    I2C_Stop();

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
    uint8_t data = readByteRTC(0x0F); // Memory address for Oscillator Stopped Flag (OSF)on DS3232; RAM register on DS1307
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
            BIT_CLEAR(Flag, typeRTC); // Clears RTC type flag to DS3232
            return 1;                 // Returns a one to start the clock/fill the clock
        } else {
            BIT_SET(Flag, typeRTC);   // Sets RTC type flag to DS1307
        }
    } 
    return 0;                         // If OSF is clear or clock is DS1307 return 0 (if any clock is working);
}

// Gets time to display
void getTime(void) {
    uint8_t seconds, minutes, hours;
    seconds = readByteRTC(0x00); // Reads only seconds from RTC to check if update needs to be performed
    // Checks to see if RTC is stopped for some reason
    if(BIT_CHECK(seconds,7)) {
        BIT_SET(ErrFlag, ErrRTC);
    } else {
        // Only updates display if its a new second, to avoid unnecessary instruction cycles
        if(seconds != oldSeconds) {
            reqReadRTC(0x01);          // Minutes address
            minutes = I2C_ReadByte();  // Reads minutes and 
            I2C_SendConfirmation(ACK); // sends ACK to increment to hours address
            hours = I2C_ReadByte();    // Reads hours
            endReadRTC();
            tubes[0] = (seconds & 0x0F); // Keeps only ones digit of seconds
            tubes[1] = (SWAP_NIBBLES(seconds) & 0x0F); // Swaps nibbles to get tens digit of seconds
            tubes[3] = (minutes & 0x0F); // Keeps only ones digit of minutes
            tubes[4] = (SWAP_NIBBLES(minutes) & 0x0F); // Swaps nibbles to get tens digit of minutes
            tubes[6] = (hours & 0x0F); // Keeps only ones digit of hours
            tubes[7] = (SWAP_NIBBLES(hours) & 0x0F); // Swaps nibbles to get tens digit of hours       
            tubes[2] = tubes[5] = 10; // Displays blanks
            // Swaps decimal points between left and right depending if its a new second. 
            if(BIT_CHECK(seconds,0)) {
                leftDP = 0x00;      // disables decimal points in all tubes
                rightDP = 0x24;     // 0x24 == 0b00100100 enables decimal point on tubes 2 and 5
            } else {
                leftDP = 0x24;  // 0x24 == 0b00100100 enables decimal point on tubes 2 and 5
                rightDP = 0x00;  // disables decimal points in all tubes
            }
            if(seconds == BCD_DATE) { // 30 in BCD
                getDate(); // Displays current date
                __delay_ms(4000); // Delay four seconds for date display
            } else if (seconds == BCD_ANIM) { // 59 in BCD
                slidingTimeAnimation();  // Slides time in at top of the hour
            } else {
                loadDisplay(); // Loads time to display
            }
            oldSeconds = seconds; // Stores seconds to oldSeconds for next time comparison
        }
    }
}

void getDate(void) {
    uint8_t day, month, year;
    reqReadRTC(0x04);
    day = I2C_ReadByte();
    I2C_SendConfirmation(ACK);
    month = I2C_ReadByte();
    I2C_SendConfirmation(ACK);
    year = I2C_ReadByte();
    endReadRTC();
    tubes[7] = (SWAP_NIBBLES(day) & 0x0F); // Swaps nibbles to get tens digit of day
    tubes[6] = (day & 0x0F); // Keeps only ones digit of day
    tubes[4] = (SWAP_NIBBLES(month) & 0x0F); // Swaps nibbles to get tens digit of month    
    tubes[3] = (month & 0x0F); // Keeps only ones digit of month
    tubes[1] = (SWAP_NIBBLES(year) & 0x0F); // Swaps nibbles to get tens digit of years
    tubes[0] = (year & 0x0F); // Keeps only ones digit of years      
    tubes[2] = tubes[5] = 10;
    leftDP = rightDP = 0x00;
    loadDisplay();
}