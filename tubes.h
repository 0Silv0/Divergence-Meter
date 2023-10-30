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
void displayError666(void);
void send1ToDrivers(void);
void send0ToDrivers(void);
void passTubeNum(unsigned char tmp7, unsigned char tmp6, unsigned char tmp5, unsigned char tmp4, unsigned char tmp3, unsigned char tmp2, unsigned char tmp1, unsigned char tmp0, unsigned char tmpLDP, unsigned char tmpRDP);

#endif	/* TUBES_H */

