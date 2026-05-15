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
#define _XTAL_FREQ 4000000           // PIC is using internal oscillator, Running at 4MHz
#define LONG_PRESS_TIME_MS 500       // Time in ms to differentiate between short and long button presses
#define TIMER_TIMEOUT 3000           // Timeout timer for settings menu, adjust as needed since it is based on loop iterations and not actual time
#define TUBE_MEMORY_START 0x22       // Starting memory address for tubes array, I chose 0x22 for the PIC16F628A because it is the first address after the 25 byte general purpose register area and it allows us to use all 8 bytes for the tubes array
#define ANIMATION_DELAY_MS 150       // Delay in ms between tube updates during slidingTimeAnimation
#define LFSR_SEED 0xAC               // Must be a non-zero number, change for different sequence
#define RANDOM_TUBE_SPIN_SPEED 80    // Controls how quickly the tubes cycle through random numbers in randomTubeAnimation (lower is faster)
#define RANDOM_TUBE_SPIN_DURATION 50 // Controls the duration of how long the tubes cycle through random numbers in randomTubeAnimation

// I2C speed
#define I2C_SPEED 100               //kbps (maximum DS1307 supports, and because we want to maximize THOUGHPUT)
#define HalfBitDelay 500/I2C_SPEED  //usec (half bit delay for timing I2C signals, calculated from I2C speed)

#endif	/* VARS_H */