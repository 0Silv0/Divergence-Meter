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
#define TUBE_MEMORY_START 0x22       // Starting memory address for tubes array, I chose 0x22 for the PIC16F628A because it is the first address after the 25 byte general purpose register area and it allows us to use all 8 bytes for the tubes array
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

//PORTA (RA4-RA7 not used, except RA5 for programmer)
//#define SCL  PORTAbits.RA0          // SCL clock line on RA0 for I2C
//#define SDA  PORTAbits.RA1          // SDA data line on RA1 for I2C
//#define BTN1 PORTAbits.RA2          // Button 1 on RA2
//#define BTN2 PORTAbits.RA3          // Button 2 on RA3

//PORTB (RB6-RB7 are for programmer)
//#define LED _PORTB_RB0_POSITION          // Debug LED on RB0
//#define HVE _PORTB_RB1_POSITION          // High voltage supply enable pin on RB1
//#define CLK _PORTB_RB2_POSITION          // Clock line to serial chips on RB2
//#define NBL _PORTB_RB3_POSITION          // NOT blanking line to serial chips on RB3
//#define DAT _PORTB_RB4_POSITION          // Data line to serial chips on RB4
//#define NLE _PORTB_RB5_POSITION          // NOT Latch Enable to serial chips on RB5

//Macros
#define BIT_CHECK(data,bitNum) ((data)>>(bitNum) & 1)                // Checks if bit is set at bitNum location
#define BIT_SET(data,bitNum) ((data) |= (1<<(bitNum)))               // Sets bit of byte at bitNum location
#define BIT_CLEAR(data,bitNum) ((data) &= ~(1<<(bitNum)))            // Clears bit of byte at bitNum location
#define BIT_TOGGLE(data,bitNum) ((data) ^= (1<<(bitNum)))            // Toggles bit of byte at bitNum location
#define SWAP_NIBBLES(data) ((data & 0x0F) << 4 | (data & 0xF0) >> 4) // Swaps nibbles of byte

#endif	/* VARS_H */