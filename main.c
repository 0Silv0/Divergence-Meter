#include "headers.h"
//TODO
//Error handling in I2C
//User input for clock settings
//Displaying numbers in nixie tubes
//  -interfacing with the serial chips

//PORTA (RA4-RA7 not used, except RA5 for programmer)
//SCL           SCL clock line on RA0 for I2C
//SDA           SDA data line on RA1 for I2C
//BTN1          Button 1 on RA2
//BTN2          Button 2 on RA3

//PORTB (RB6-RB7 are for programmer)
//LED           Debug LED on RB0
//HVE           High voltage supply enable pin on RB1
//CLK           Clock line to serial chips on RB2
//NBL           NOT blanking line to serial chips on RB3
//DAT           Data line to serial chips on RB4
//NLE           NOT Latch Enable to serial chips on RB5

//Flag bits
// short1       0   Short BTN1 press
// long1        1   Long BTN1 press
// short2       2   Short BTN2 press
// long2        3   Long BTN2 press
// typeRTC      4   RTC type, 0 for DS3232 1 for DS1307
// Slide        5   Slide loading used in ___ (1=slide)
// AMPM         6   Keeps track of AM/PM (1=PM)
// Clk12        7   12/24 hour preference (1=12 hour clock)

//Error Flag bits
// ErrNACK      0   RTC NACK/SDA line high
// Global variables
unsigned char Flag;
unsigned char ErrFlag;
unsigned char PORTA_SHADOW;
unsigned char PORTB_SHADOW;

void main(void) {
    Init(); // Initialize everything
    unsigned char menu = 1;
    while(1) {
        if(menu) {
            if(!(BIT_CHECK(ErrFlag, ErrRTC))) {
                getTime();
            } else {
                displayError();
            }
        }
        if(BTN1) {
            while(BTN1) {}  // wait for btn release
            menu = 0;
            //passTubeNum(BIT_CHECK(ErrFlag, ErrNACK),10,1,2,3,4,5,6,0x00,0x00); using this for variable testing
        } else if (BTN2) {
            while(BTN2) {}  // wait for btn release
            menu = 1;
        }
    }
}

// Initializations
void Init(void) {
    CMCON = 0b111; //  Disables comparators and enables digital I/O
    TRISA = 0b00101111; // Set I/O pins in PORTA
    TRISB = 0b00000000; // Set all pins to output in PORTB
    OPTION_REG = 0b11011111; // Set TMR0 to clock of the internal clock, no prescaler
    PORTA_SHADOW = 0x00; // Clear PORTA Shadow bits
    PORTB_SHADOW = 0x00; // Clear PORTB Shadow bits
    PORTB = PORTB_SHADOW;
    Flag = 0x10; // Clears flag bits (sets default RTC to DS1307)
    ErrFlag = 0x00; // Clears error flag
    InitI2C(); // Initialize I2C
    InitTubes(); // Initialize tubes
    // Checks to see if clock is running
    if(isRTCRunning()) { // Clock is DS1307 and is currently stopped
        BIT_SET(Flag, typeRTC); // Sets RTC type flag to DS1307
        startRTC(); // Start DS1307
    } else { // DS1307 is either running or the clock is a DS3232
        if(checkRTCType()) { // returns 1 for stopped DS3232 and 0 for everything else
            startRTC(); // fills DS3232 after it was started in isRTCRunning()
        }
    }
}

// Swaps nibbles in a byte
unsigned char swapNibbles(unsigned char data) {
    return ((data & 0x0F) << 4 | (data & 0xF0) >> 4);
}