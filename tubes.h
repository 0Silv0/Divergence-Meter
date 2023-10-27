#ifndef TUBES_H
#define	TUBES_H

// Function declarations
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

#endif	/* TUBES_H */

