/**
 * @file settings.h
 * @author silv
 * @date 2026-05-13
 * @brief Settings menu and related function declarations
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <xc.h>
#include <stdint.h>

void setBrightness(uint8_t brightnessLevel);
uint8_t getBrightness(void);

#endif /* SETTINGS_H */