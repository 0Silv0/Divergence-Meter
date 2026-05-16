/**
 * @file tubes.h
 * @author silv
 * @date 2026-05-13
 * @brief Tube control function declarations
 */
#ifndef TUBES_H
#define TUBES_H

 #include <xc.h>
 #include <stdint.h>

 typedef struct {
    uint8_t tubes[8];
    uint8_t leftDP;
    uint8_t rightDP;
 } Display;

 void InitTubes(void);

#endif /* TUBES_H */