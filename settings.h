#ifndef SETTINGS_H
#define	SETTINGS_H

// Function Declarations
void settingsMenu(unsigned char menu);
void hourFormatSetting(void);
void hoursSetting(void);
void minuteSetting(void);
void daySetting(void);
void monthSetting(void);
void yearSetting(void);
void dateFormatSetting(void);
void blankingSetting(void);
void unblankingSetting(void);
void timeAdjSetting(void);
void brightnessSetting(void);
void buttons(void);
void incDecBCD(void);

// Global variables
extern unsigned char blankStart;
extern unsigned char blankEnd;
#endif	/* SETTINGS_H */