#ifndef MAIN_H
#define	MAIN_H

// PIC16F628A Configuration Bit Settings

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//PIC is using internal oscillator
#define _XTAL_FREQ 4000000          //Running at 4MHz

//Bit Masks
#define clearPORTA_SHADOW PORTA_SHADOW &= 0x00 // Clears PORTA Mask
#define clearPORTB_SHADOW PORTB_SHADOW &= 0x00 // Clears PORTB Mask

//PORTA (RA0 = SCL RA1 = SDA for I2C, RA4-RA7 not used, except RA5 for programmer)
#define BTN1 PORTAbits.RA2         // Button 1 on RA2
#define BTN2 PORTAbits.RA3         // Button 2 on RA3

//PORTB (RB6-RB7 are for programmer)
#define LED _PORTB_RB0_POSITION          // Debug LED on RB0
#define HVE _PORTB_RB1_POSITION          // High voltage supply enable pin on RB1
#define CLK _PORTB_RB2_POSITION          // Clock line to serial chips on RB2
#define NBL _PORTB_RB3_POSITION          // NOT blanking line to serial chips on RB3
#define DAT _PORTB_RB4_POSITION          // Data line to serial chips on RB4
#define NLE _PORTB_RB5_POSITION          // NOT Latch Enable to serial chips on RB5

//Macros
#define BIT_CHECK(data,bitNum) ((data) & 1<<(bitNum)) // Checks if bit is set at bitNum location
#define BIT_SET(data,bitNum) (data |= (1<<bitNum))    // Sets bit of byte at bitNum location
#define BIT_CLEAR(data,bitNum) (data &=(0<<bitNum))   // Clears bit of byte at bitNum location
#define BIT_TOGGLE(data,bitNum) (data ^= (1<<bitNum)) // Toggles bit of byte at bitNum location

#endif	/* MAIN_H */

