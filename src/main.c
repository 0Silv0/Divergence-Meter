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
    TRISA = 0b00001100;   // RA2=input (BTN1), RA3=input (BTN2), all others output
    TRISB = 0b11010010;   // RB1=input (SDA), RB4=input (SCL), RB6=input (PGC), RB7=input (PGD), rest output
    LATA = 0x00;          // All outputs low — HVE off, DAT low, NBL low
    LATB = 0x00;          // All outputs low — LED off, CLK low, NLE low
    ANSELA = 0x00;        // All PORTA pins digital
    ANSELB = 0x00;        // All PORTB pins digital
    CM1CON0 = 0x00;       // Disable comparater 1
    CM2CON0 = 0x00;       // Disable comparater 2
    CM1CON1 = 0x00;       // Clear comparater 1 i/o routing
    CM2CON1 = 0x00;       // Clear comparater 2 i/o routing

    I2C_Init();           // Initialize I2C for RTC communication
    InitTubes();          // Initalize tubes to blank display and start PWM for brightness control
}

// Delay function in milliseconds
void delayMs(uint16_t ms) {
    uint16_t i;
    for (i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}