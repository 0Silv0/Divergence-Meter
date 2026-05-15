/**
 * @file config.h
 * @author silv
 * @date 2026-05-15
 * @brief Configuration bit settings for PIC16F1846
 */
#ifndef CONFIG_H
#define CONFIG_H

// PIC16F1846 Configuration Bit Settings
// CONFIG words are located at 0x8007 (CONFIG1) and 0x8008 (CONFIG2)

 #include <xc.h>

// =============================================================================
// CONFIG1
// =============================================================================

// FOSC — Oscillator Selection
//   INTOSC   : Internal oscillator; I/O on CLKIN pin
//   EXTRC    : External RC oscillator; CLKOUT on CLKIN pin
//   ECL      : External clock, Low power (0–0.5 MHz)
//   ECM      : External clock, Medium power (0.5–4 MHz)
//   ECH      : External clock, High power (4–32 MHz)
//   XT       : Crystal oscillator (0.5–4 MHz)
//   HS       : High-speed crystal/resonator (4–32 MHz)
//   LP       : Low-power crystal (max 32 kHz)
#pragma config FOSC = INTOSC

// WDTE — Watchdog Timer Enable
//   ON       : WDT always enabled
//   NSLEEP   : WDT enabled in active mode, disabled in Sleep
//   SWDTEN   : WDT controlled by SWDTEN bit in WDTCON
//   OFF      : WDT disabled
#pragma config WDTE = OFF

// PWRTE — Power-up Timer Enable
//   ON       : 64 ms power-up delay enabled (helps VDD stabilise)
//   OFF      : No power-up delay
#pragma config PWRTE = ON

// MCLRE — MCLR Pin Function
//   ON       : MCLR/VPP pin is the hardware reset pin
//   OFF      : MCLR/VPP pin is a digital input (RA5); no external reset
// Will keep this on to allow external reset; this pin is not going to be utilized as an input regardless
#pragma config MCLRE = ON

// CP — Flash Program Memory Code Protection
//   ON       : Code protected (cannot be read back)
//   OFF      : No code protection
#pragma config CP = OFF

// CPD — Data Memory (EEPROM) Code Protection
//   ON       : EEPROM protected
//   OFF      : No EEPROM protection
#pragma config CPD = OFF

// BOREN — Brown-out Reset Enable
//   ON       : BOR always enabled
//   NSLEEP   : BOR enabled in active mode, disabled in Sleep
//   SBODEN   : BOR controlled by SBOREN bit in BORCON
//   OFF      : BOR disabled
#pragma config BOREN = ON

// CLKOUTEN — Clock Out Enable
//   ON       : CLKOUT function on RA4/OSC2 pin (FOSC/4 output)
//   OFF      : RA4/OSC2 is a normal I/O pin
#pragma config CLKOUTEN = OFF

// IESO — Internal/External Switchover
//   ON       : Switchover mode enabled (start on INTOSC, switch to ext osc)
//   OFF      : No switchover
// Using INTOSC only, switchover not needed
#pragma config IESO = OFF

// FCMEN — Fail-Safe Clock Monitor Enable
//   ON       : FSCM monitors clock; switches to INTOSC if ext clock fails
//   OFF      : FSCM disabled
// No external clock to monitor
#pragma config FCMEN = OFF

// =============================================================================
// CONFIG2
// =============================================================================

// WRT — Flash Self-Write Protection
//   ALL      : 000h–7FFh write-protected
//   HALF     : 000h–3FFh write-protected
//   BOOT     : 000h–1FFh write-protected (boot block only)
//   OFF      : No write protection
#pragma config WRT = OFF

// VCAPEN — Voltage Regulator Capacitor Enable (RA6/VCAP pin)
//   ON       : VCAP pin enabled for internal regulator capacitor
//   OFF      : VCAP pin is digital I/O (RA6)
#pragma config VCAPEN = OFF

// PLLEN — 4× PLL Enable
//   ON       : 4× PLL always on when FOSC = INTOSC (gives up to 32 MHz)
//   OFF      : PLL disabled; SPLLEN bit in OSCCON controls it in software
// PIC will be running at 32MHz
#pragma config PLLEN = ON

// STVREN — Stack Over/Underflow Reset Enable
//   ON       : Stack overflow/underflow causes a Reset
//   OFF      : Stack overflow/underflow sets flags only
#pragma config STVREN = ON

// BORV — Brown-out Reset Voltage Selection
//   HI       : ~2.7 V trip point (higher threshold)
//   LO       : ~1.9 V trip point (lower threshold)
// Circuit is running at 5v, so use the high threshold
#pragma config BORV = LO

// LVP — Low-Voltage Programming Enable
//   ON       : LVP enabled; RB3/PGM is the programming pin
//   OFF      : HV on MCLR required for programming; RB3 is normal I/O
// Using MPLAB Snap because the PICkit 3 bricked itself, so LVP is needed; Whole reason I switched to the PIC16F1846
#pragma config LVP = ON

#endif /* CONFIG_H */