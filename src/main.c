/**
 * @file main.c
 * @author silv
 * @date 2026-05-13
 * @brief Main function
 */

#include "../inc/main.h"
#include "../inc/vars.h"
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
    InitPWM(); // Initialize PWM for tube brightness control
}

// Delay function in milliseconds
void delayMs(uint16_t ms) {
    uint16_t i;
    for (i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}