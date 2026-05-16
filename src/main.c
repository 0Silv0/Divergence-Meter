/**
 * @file main.c
 * @author silv
 * @date 2026-05-13
 * @brief Main function
 */

#include "../inc/main.h"
#include "../inc/vars.h"
#include "../inc/i2c.h"
#include "../inc/tubes.h"

// Main function
int main(){
    Init(); // Initialize peripherals and settings
    
    // Main loop
    while(1) {

    }
    return 0;
}

// PIC Initialization
void Init(void) {
    OSCCON = 0b11110000;  // IRCF=1111 (8MHz), SCS=00 (use FOSC config), PIC Oscillator set to 8MHz internal with PLL for 32MHz operation
    InitPWM(); // Initialize PWM for tube brightness control
    I2C_Init(); // Initialize I2C for RTC communication
}

// Delay function in milliseconds
void delayMs(uint16_t ms) {
    uint16_t i;
    for (i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}