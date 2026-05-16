/**
 * @file rtc.h
 * @author silv
 * @date 2026-05-15
 * @brief Real-Time Clock function declarations
 */

#ifndef RTC_H
#define RTC_H

#include <xc.h>
#include <stdint.h>

void startRTC(void);
__bit isRTCHalted(void);
__bit checkRTCType(void);
void getTime(void);
void getDate(void);

#endif /* RTC_H */