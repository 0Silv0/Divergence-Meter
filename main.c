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

void main(void) {
    // Initializations
    CMCON = 0b111; //  Disables comparators and enables digital I/O
    TRISA = 0b00101111; // Set I/O pins in PORTA
    TRISB = 0b00000000; // Set all pins to output in PORTB
    OPTION_REG = 0b11011111; // Set TMR0 to clock of the internal clock, no prescaler
    STATUSbits.RP0 = 1; // Goes to...
    STATUSbits.RP1 = 0; // Bank 1
    InitI2C(); 
    // Checks to see if clock is running
    if(isRTCRunning()) { // Clock is DS1307 and is currently stopped
        startRTC(); // Start DS1307
    } else { // DS1307 is either running or the clock is a DS3232
        if(checkRTCType()) { // returns 1 for stopped DS3232 and 0 for everything else
            startRTC(); // fills DS3232 after it was started in isRTCRunning()
        }
    }
    // End initializations 
//    while(1) {
//        // If BTN1:2 are pressed
//        if(BTN1 && BTN2) {
//            LED = 1; // enable LED
//        } else {
//            LED = 0; // disable LED
//        }
//    }
    return;
}
