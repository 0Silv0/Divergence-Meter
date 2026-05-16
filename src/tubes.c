/**
 * @file tubes.c
 * @author silv
 * @date 2026-05-14
 * @brief Tube control functions
 */

#include "../inc/tubes.h"
#include "../inc/vars.h"
#include "../inc/settings.h"

// Initialize PWM for tube brightness control
void InitPWM(void) {
    TRISAbits.TRISA4  = 0;          // RA4 as output
    PR2 = 255;                      // PWM period
    CCP4CONbits.CCP4M = 0b1100;     // PWM mode
    T2CONbits.T2CKPS  = 0b00;       // Prescaler 1:1
    T2CONbits.TMR2ON  = 1;          // Timer2 on
    setBrightness(7);               // Default brightness mode 7
}