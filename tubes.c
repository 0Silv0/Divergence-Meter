#include "headers.h"

// Once I have a better understanding of writing to the serial to parallel drivers
// I will try and not use absolute variable declarations, so as to stay out of the 
// compilers way. Started at a different location than Tom because I did not want to 
// fragment the GPR stack and maybe cause issues with the linker.
unsigned char leftDP;
unsigned char rightDP;
unsigned char T0 __at(0x22);
unsigned char T1 __at(0x23);
unsigned char T2 __at(0x24);
unsigned char T3 __at(0x25);
unsigned char T4 __at(0x26);
unsigned char T5 __at(0x27);
unsigned char T6 __at(0x28);
unsigned char T7 __at(0x29);

// Loads Steins;Gate worldline number
void preLoadWL(void) {
    T7 = 1;
    T6 = 10; // Display no number
    T5 = 0;
    T4 = 4;
    T3 = 8;
    T2 = 5;
    T1 = 9;
    T0 = 0;
    leftDP = 0x00; // Clears leftDP
    rightDP = 0x00; // Clears rightDP
    BIT_SET(rightDP, 6); // Sets right decimal point for tube 6
}
// Loads numbers into display before enabling latches
void loadDisplay(void) {
    unsigned char *tubePtr;
    tubePtr = &T7; // Points to memory address of T7
    unsigned char dispNum;
    unsigned char LDP = leftDP; // Copy leftDP so we do not destroy original value
    unsigned char RDP = rightDP; // Copy rightDP so we do not destroy original value
    
   for (unsigned char tubeIndex = 0; tubeIndex <= 7; tubeIndex++) {  
        checkDP(&LDP); // Checks left decimal point
        dispNum = 1;
        // Loops through all 9 numbers and sends 0 or 1 if it matches the number we want to display
        for (unsigned char numIndex = 0; numIndex < 9; numIndex++) {
            if(dispNum == *tubePtr) {
                send1ToDrivers();
            } else {
                send0ToDrivers();
            }
            dispNum++;
        }
        // Checks to see if number we want to display is 0
        if (*tubePtr == 0) { 
            send1ToDrivers();
        } else {
            send0ToDrivers();
        }
        checkDP(&RDP); // Checks right decimal point
        tubePtr--;
    }
    latch(); // Latches results
}

// Rotates DP flag, when carry bit will be 1 returns true. Essentially RLF in assembly. Just doesn't save carry bit and it doesn't need to :)
void checkDP(unsigned char *DP) {
    if (BIT_CHECK((*DP), 7)) {
        // Rotation would've put 1 in carry flag in assembly, so send one to drivers
        *DP = (*DP) << 1;
        send1ToDrivers();
    } else {
        // Rotates DP left one and sends zero to drivers
        *DP = (*DP) << 1;
        send0ToDrivers();
    }
}

void latch(void) {
    BIT_SET(PORTB_SHADOW, NLE); // Latches load when NLE is high
    PORTB = PORTB_SHADOW;
    BIT_CLEAR(PORTB_SHADOW, NLE); // Latches lock when NLE is low
    PORTB = PORTB_SHADOW;
}

// Displays loaded numbers into tubes
void display(void) {
    BIT_SET(PORTB_SHADOW, NBL); // Set NOT blanking high, causing tubes to no longer be blanked
    BIT_SET(PORTB_SHADOW, HVE); // Set high voltage enable on to display tubes
    PORTB = PORTB_SHADOW;
}
//Flashes full brightness in tubes
void flashBrightness(void) {
    
}

// Blanks eight tubes
void blankTubes(void) {
    unsigned char *ptr;
    ptr = &T0; // Points to memory address of T0
    // Assigns 10 to all eight tubes
    for(unsigned char i = 0; i < 8; i++) {
        *ptr = 10; // 10 will display blank in tube
        ptr++; // Goes to next tube address
    }
}

// Displays error 666
void displayError666(void) {
    blankTubes(); // Clear tubes
    T0 = T1 = T2 = 6; // Sets T0-T2 to 6
    leftDP = rightDP = 0x00; // Clears left and right decimal points
    loadDisplay(); // Sends tube data to serial to parallel drivers
    display(); // Displays sent data
    // Move to error handler
    while(1){
        // Either clock is not working, or no battery is installed.
        // Infinite loop crash, GG.
    }
}

// Sends 1 to serial to parallel drivers
void send1ToDrivers(void) {
    BIT_SET(PORTB_SHADOW, DAT); // Set Data line high for one
    BIT_SET(PORTB_SHADOW, CLK); // Set clock high
    PORTB = PORTB_SHADOW;
    BIT_CLEAR(PORTB_SHADOW, CLK); // Set clock low
    PORTB = PORTB_SHADOW;
}

// Sends 0 to serial to parallel drivers
void send0ToDrivers(void) {
    BIT_CLEAR(PORTB_SHADOW, DAT); // Set Data line low for zero
    BIT_SET(PORTB_SHADOW, CLK); // Set clock high
    PORTB = PORTB_SHADOW;
    BIT_CLEAR(PORTB_SHADOW, CLK); // Set clock low
    PORTB = PORTB_SHADOW;    
}