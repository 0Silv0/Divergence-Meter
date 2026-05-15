/**
 * @file settings.c
 * @author silv
 * @date 2026-05-13
 * @brief Settings menu and related functions
 */

#include "../inc/settings.h"
#include <stdint.h>

static uint8_t tubeBrightness; // Global variable to track current brightness level (1-8)
static const uint8_t brightnessLUP[8] = {32, 64, 96, 128, 160, 192, 224, 255}; // Lookup table for brightness levels (1-8)

// Sets tube brightness level (1-8), 8 is resevered for flashing tubes. 7 is maximum brightness to prevent cathode poisoning.
void setBrightness(uint8_t brightnessLevel) {
    tubeBrightness = brightnessLevel;
    // CHANGE CCP to correct one when pwm pin is decided
    CCPR1L = brightnessLUP[brightnessLevel - 1];  // level-1 since table is 0 indexed
}

// Returns current tube brightness level
uint8_t getBrightness(void) {
    return tubeBrightness;
}