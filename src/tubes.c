/**
 * @file tubes.c
 * @author silv
 * @date 2026-05-14
 * @brief Tube control functions
 */

#include "../inc/tubes.h"
#include "../inc/vars.h"
#include "../inc/settings.h"

// Initialize PWM for tube brightness control (Adjust later when I decide which pin to use for NBL, can't be RB3 as it is used for LVP)
void InitPWM(void) {
    PR2 = 255;                      // PWM period
    setBrightness(7);               // Default brightness mode 7
    CCP1CONbits.CCP1M = 0b1100;     // PWM mode
    T2CONbits.TMR2ON  = 1;          // Timer2 on
    T2CONbits.T2CKPS  = 0b00;       // Prescaler 1:1
    TRISBbits.TRISB3  = 0;          // RB3 as output — adjust to wherever NBL is
}