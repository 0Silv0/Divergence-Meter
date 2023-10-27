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
// Done         4   World line animation complete
// Slide        5   Slide loading used in ___ (1=slide)
// AMPM         6   Keeps track of AM/PM (1=PM)
// Clk12        7   12/24 hour preference (1=12 hour clock)

// Global variables
unsigned char flag;
unsigned char PORTA_SHADOW;
unsigned char PORTB_SHADOW;

void main(void) {
    Init(); // Initialize everything
    preLoadWL();
    loadDisplay();
    display();
    while(1) {
        
    }
//    while(1) {
//        // If BTN1:2 are pressed
//        if(BTN1 && BTN2) {
//            LED = 1; // enable LED
//        } else {
//            LED = 0; // disable LED
//        }
//    }
}

// Initializations
void Init(void) {
    CMCON = 0b111; //  Disables comparators and enables digital I/O
    TRISA = 0b00101111; // Set I/O pins in PORTA
    TRISB = 0b00000000; // Set all pins to output in PORTB
    OPTION_REG = 0b11011111; // Set TMR0 to clock of the internal clock, no prescaler
    PORTA_SHADOW = 0b00000000; // Clear PORTA Shadow bits
    PORTB_SHADOW = 0b00000000; // Clear PORTB Shadow bits
    PORTB = PORTB_SHADOW;
    flag = 0b00000000; // Clears flag bits
    InitI2C(); // Initialize I2C
    // Checks to see if clock is running
    if(isRTCRunning()) { // Clock is DS1307 and is currently stopped
        startRTC(); // Start DS1307
    } else { // DS1307 is either running or the clock is a DS3232
        if(checkRTCType()) { // returns 1 for stopped DS3232 and 0 for everything else
            startRTC(); // fills DS3232 after it was started in isRTCRunning()
        }
    }
}