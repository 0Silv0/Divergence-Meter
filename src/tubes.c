/**
 * @file tubes.c
 * @author silv
 * @date 2026-05-14
 * @brief Tube control functions
 */

#include "../inc/tubes.h"
#include "../inc/vars.h"
#include "../inc/settings.h"

static Display tubeDisplay;

// Forward declarations for static helpers
static void blankTubes(void);
static void loadDisplay(void);
static void checkDP(uint8_t *DP);
static void sendBitToDrivers(uint8_t bit);
static void latch(void);
static void InitPWM(void);

// * Nixie tube display local helper functions *

// Blanks eight tubes
static void blankTubes(void) {
    uint8_t i;
    // Assigns 10 to all eight tubes so they won't display a digit
    for(i = 0; i < 8; i++) tubeDisplay.tubes[1] = 10;
    tubeDisplay.leftDP = tubeDisplay.rightDP = 0x00; // Clears left and right decimal points for the display
}

// Loads numbers into display before enabling latches
static void loadDisplay(void) {
    uint8_t dispNum, numIndex;
    uint8_t LDPtmp, RDPtmp;
    int8_t i;

    LDPtmp = tubeDisplay.leftDP;  // Copy leftDP so we do not destroy original value
    RDPtmp = tubeDisplay.rightDP; // Copy rightDP so we do not destroy original value

    for(i = 7; i >= 0; i--) {
        checkDP(&LDPtmp); // Checks left decimal point
        dispNum = 1;
        // Loops through all 9 numbers and sends 0 or 1 if it matches the number we want to display
        for (numIndex = 0; numIndex < 9; numIndex++) {
            sendBitToDrivers(dispNum == tubeDisplay.tubes[i] ? 1 : 0);
            dispNum++;
        }
        // This check is necessary because zero comes last on the tubes, so if we want to display zero we need to send a one to the drivers after the loop
        // Checks to see if number we want to display is 0
        sendBitToDrivers(tubeDisplay.tubes[i] == 0 ? 1 : 0);
        checkDP(&RDPtmp); // Checks right decimal point
    }
    latch(); // Latches results
}

// Rotates DP flag, when carry bit will be 1 returns true. Essentially RLF in assembly. Just doesn't save carry bit and it doesn't need to :)
static void checkDP(uint8_t *DP) {
    uint8_t bit = BIT_CHECK(*DP, 7);
    *DP = (uint8_t)((*DP) << 1); // Rotate left by one bit for next tube
    sendBitToDrivers(bit); // Send bit to drivers, if bit is 1 then send one, if bit is 0 then send zero
}

// Sends a one or zero to the serial to parallel drivers
static void sendBitToDrivers(uint8_t bit) {
    if(bit) {
        BIT_SET(LATA, DAT);   // Set Data line high for one
    } else {
        BIT_CLEAR(LATA, DAT); // Set Data line low for zero
    }
    BIT_SET(LATB, CLK);       // Set clock high
    __delay_us(1);            // Hold clock high long enough for shift registers to latch
    BIT_CLEAR(LATB, CLK);     // Set clock low
}

// Enables latches in serial to parallel drivers
static void latch(void) {
    BIT_SET(LATB, NLE);   // Latches load when NLE is high
    __delay_us(1);        // Hold latch enable high long enough for parallel load
    BIT_CLEAR(LATB, NLE); // Latches lock when NLE is low
}

// Initialize PWM for tube brightness control
static void InitPWM(void) {
    TRISAbits.TRISA4  = 0;          // RA4 as output
    PR2 = 255;                      // PWM period
    CCP4CONbits.CCP4M = 0b1100;     // PWM mode
    T2CONbits.T2CKPS  = 0b00;       // Prescaler 1:1
    T2CONbits.TMR2ON  = 1;          // Timer2 on
    setBrightness(7);               // Default brightness mode 7
}

// Initialize nixie tube display
void InitTubes(void) {
    InitPWM();          // Initalizes PWM for tube brightness
    BIT_SET(LATB, HVE); // Turn on high voltage PSU
    blankTubes();       // Initialize all tubes to display nothing
    loadDisplay();      // Displays empty tubes
}