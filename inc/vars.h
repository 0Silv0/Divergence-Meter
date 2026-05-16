/**
 * @file vars.h
 * @author silv
 * @date 2026-05-13
 * @brief Variable declarations
 */

#ifndef VARS_H
#define	VARS_H

#include <xc.h>
#include <stdint.h>

// User defined variables
// =============================================================================
// Convenience macro — set _XTAL_FREQ to match your OSCCON speed setting
//      4 MHz INTOSC:  OSCCON = 0b01101000;  (IRCF = 1101)
//      8 MHz INTOSC:  OSCCON = 0b01110000;  (IRCF = 1110)
//     32 MHz (PLL):   OSCCON = 0b11110000;  (IRCF = 1111) + PLLEN = ON
// =============================================================================
#define _XTAL_FREQ 32000000UL        // 8 MHz INTOSC × 4 PLL = 32 MHz actual speed
#define LONG_PRESS_TIME_MS 500       // Time in ms to differentiate between short and long button presses
#define TIMER_TIMEOUT 3000           // Timeout timer for settings menu, adjust as needed since it is based on loop iterations and not actual time
#define ANIMATION_DELAY_MS 150       // Delay in ms between tube updates during slidingTimeAnimation
#define LFSR_SEED 0xAC               // Must be a non-zero number, change for different sequence
#define RANDOM_TUBE_SPIN_SPEED 80    // Controls how quickly the tubes cycle through random numbers in randomTubeAnimation (lower is faster)
#define RANDOM_TUBE_SPIN_DURATION 50 // Controls the duration of how long the tubes cycle through random numbers in randomTubeAnimation

// -----------------------------------------------------------------------------
// I2C bus speed
// Baud rate formula: SSPADD = (Fosc / (4 * I2C_CLOCK_HZ)) - 1
// At 32 MHz and 400 kHz: SSPADD = (32000000 / (4 * 400000)) - 1 = 19
// At 32 MHz and 100 kHz: SSPADD = (32000000 / (4 * 100000)) - 1 = 79
// -----------------------------------------------------------------------------
#define I2C_CLOCK_SPEED 100000UL    // 100000UL or 400000UL (RTC runs at 100 kHz, might change to  a different RTC that supports 400 kHz in the future)
#define I2C_SSPADD      ((uint8_t)(_XTAL_FREQ / (4UL * I2C_CLOCK_SPEED)) - 1)

// I2C definitions
#define SDA TRISBbits.TRISB1         // SDA data line on RB1 for I2C
#define SCL TRISBbits.TRISB4         // SCL clock line on RB4 for I2C
#define SEN SSP1CON2bits.SEN         // Start condition enable bit
#define RSEN SSP1CON2bits.RSEN       // Repeated start condition enable bit
#define PEN SSP1CON2bits.PEN         // Stop condition enable bit
#define ACKEN SSP1CON2bits.ACKEN     // Acknowledge sequence enable bit
#define ACKDT SSP1CON2bits.ACKDT     // Acknowledge data bit (1 = NACK, 0 = ACK)
#define RCEN SSP1CON2bits.RCEN       // Receive enable bit
#define ACKSTAT SSP1CON2bits.ACKSTAT // Acknowledge status bit (1 = NACK received, 0 = ACK received)
#define BF SSP1STATbits.BF           // Buffer full status bit

// I2C codes
#define I2C_OK          0   // Device acknowledged its address
#define I2C_NACK        1   // Device did not acknowledge its address
#define I2C_TIMEOUT     2   // Bus didn't become idle in time

//PORTA
//*RA4 = NBL
//*RA6 = HVE
//*RA7 = DAT
//--------
//RA0 = FREE
//RA1 = FREE
#define BTN1 PORTAbits.RA2          // Button 1 on RA2
#define BTN2 PORTAbits.RA3          // Button 2 on RA3
#define NBL _PORTA_RA4_POSITION     // NOT blanking line to serial chips on RA4
//RA5 = MCLR                          (hardware reset, also used for programming)
#define HVE _PORTA_RA6_POSITION     // High voltage supply enable pin on RA6
#define DAT _PORTA_RA7_POSITION     // Data line to serial chips on RA7

//PORTB
#define LED _PORTB_RB0_POSITION     // Debug LED on RB0
//*RB1 = SDA                          (i2c data line)
#define CLK _PORTB_RB2_POSITION     // Clock line to serial chips on RB2
//*RB3 = LVP                          (pin is disabled because it is being used for LVP)
//*RB4 = SCL                          (i2c clock line)
#define NLE _PORTB_RB5_POSITION     // NOT Latch Enable to serial chips on RB5
//RB6 = PGC                           (clock line for programming, can be used but will avoid just in case)
//RB7 = PGD                           (data line for programming, can be used but will avoid just in case)

//Macros
#define BIT_CHECK(data,bitNum) ((data)>>(bitNum) & 1)                // Checks if bit is set at bitNum location
#define BIT_SET(data,bitNum) ((data) |= (1<<(bitNum)))               // Sets bit of byte at bitNum location
#define BIT_CLEAR(data,bitNum) ((data) &= ~(1<<(bitNum)))            // Clears bit of byte at bitNum location
#define BIT_TOGGLE(data,bitNum) ((data) ^= (1<<(bitNum)))            // Toggles bit of byte at bitNum location
#define SWAP_NIBBLES(data) ((data & 0x0F) << 4 | (data & 0xF0) >> 4) // Swaps nibbles of byte

#endif	/* VARS_H */