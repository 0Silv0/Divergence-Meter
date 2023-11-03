#ifndef TUBES_H
#define	TUBES_H

// Function declarations
void InitTubes(void);
void preLoadWL(void);
void loadDisplay(void);
void checkDP(unsigned char *DP);
void latch(void);
void display(void);
void flashBrightness(void);
void blankTubes(void);
void displayError(void);
void send1ToDrivers(void);
void send0ToDrivers(void);
//void passTubeNum(unsigned char tmp7, unsigned char tmp6, unsigned char tmp5, unsigned char tmp4, unsigned char tmp3, unsigned char tmp2, unsigned char tmp1, unsigned char tmp0, unsigned char tmpLDP, unsigned char tmpRDP);
//void settingsDisplay(unsigned char tmp7, unsigned char tmp6, unsigned char tmp1, unsigned char tmp0);

// Global variables
extern unsigned char leftDP;
extern unsigned char rightDP;
extern unsigned char T0 __at(0x22);
extern unsigned char T1 __at(0x23);
extern unsigned char T2 __at(0x24);
extern unsigned char T3 __at(0x25);
extern unsigned char T4 __at(0x26);
extern unsigned char T5 __at(0x27);
extern unsigned char T6 __at(0x28);
extern unsigned char T7 __at(0x29);

#endif	/* TUBES_H */

