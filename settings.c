
#include "headers.h"

unsigned char blankStart, blankEnd;
unsigned char incMin, incMax;

void InitSettings(void) {
    
}
// Settings menu
void settingsMenu(unsigned char menu) {
    switch(menu) {
        // Time settings grouped together
        case 0:
            hourFormatSetting(); // 12/24 Hour setting
            hoursSetting(); // Hours setting
            minuteSetting(); // Minute setting
            blankingSetting();   // Tube blanking setting
            unblankingSetting();  // Tube un-blanking setting 
            timeAdjSetting(); // Time adjustment setting
        // Date settings grouped together
        case 1:
            daySetting(); // Day setting
            monthSetting(); // Month setting
            yearSetting(); // Year setting
            dateFormatSetting(); // Date format setting
        case 2:
            brightnessSetting(); // Brightness setting
        default:
            break;               
    }
}

void hourFormatSetting(void) {
    blankTubes();
    T7 = 0;
    T6 = 1;
    do {
        if(BIT_CHECK(Flag2, Clk12)) {
            T1 = 1;
            T0 = 2;
        } else {
            T1 = 2;
            T0 = 4;
        }  
        loadDisplay();
        buttons();
        if(BIT_CHECK(Flag, short1)) {
            BIT_TOGGLE(Flag2, Clk12);
        }
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting
    
    // write to EEPROM to save 12/24 hour choice
}

void hoursSetting(void) {
    unsigned char hours;
    hours = readByteRTC(0x02);
    T6 = 2;
    do {
        T1 = (swapNibbles(hours) & 0x0F); // Keeps tens digit of hours        
        T0 = (hours & 0x0F); // Keeps ones digit of hours
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting  
    writeByteRTC(0x02, hours);
}

void minuteSetting(void) {
    unsigned char minutes;
    minutes = readByteRTC(0x01);
    T6 = 3;
    do {
        T1 = (swapNibbles(minutes) & 0x0F); // Keeps tens digit of minutes        
        T0 = (minutes & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x01, minutes);
}

void daySetting(void) {
    unsigned char day;
    day = readByteRTC(0x04);
    T6 = 4;
    do {
        T1 = (swapNibbles(day) & 0x0F); // Keeps tens digit of minutes        
        T0 = (day & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x04, day);    
}

void monthSetting(void) {
    unsigned char month;
    month = readByteRTC(0x01);
    T6 = 5;
    do {
        T1 = (swapNibbles(month) & 0x0F); // Keeps tens digit of minutes        
        T0 = (month & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x05, month);    
}

void yearSetting(void) {
    unsigned char year;
    year = readByteRTC(0x01);
    T6 = 6;
    do {
        T1 = (swapNibbles(year) & 0x0F); // Keeps tens digit of minutes        
        T0 = (year & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x06, year);    
}

void dateFormatSetting(void) {
    T6 = 7;
    do {
        T1 = 0;
        T0 = BIT_CHECK(Flag2, dateDMY);            

        loadDisplay();
        buttons();
        if(BIT_CHECK(Flag, short1)) {
            BIT_TOGGLE(Flag2, dateDMY);
        }
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    //write to eeprom
}

void blankingSetting(void) {
    blankStart = readByteRTC(0x14);
    T6 = 8;
    do {
        T1 = (swapNibbles(blankStart) & 0x0F); // Keeps tens digit of minutes        
        T0 = (blankStart & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x14, blankStart);    
}

void unblankingSetting(void) {
    blankEnd = readByteRTC(0x15);
    T6 = 9;
    do {
        T1 = (swapNibbles(blankEnd) & 0x0F); // Keeps tens digit of minutes        
        T0 = (blankEnd & 0x0F); // Keeps ones digit of minutes
        loadDisplay();
        buttons();
        incDecBCD();
    } while (!BIT_CHECK(Flag, long2)); // Wait until long press of button 2 to go to next setting    
    writeByteRTC(0x15, blankEnd);    
}

void timeAdjSetting(void) {
    
}

void brightnessSetting(void) {
    
}

//Returns if BTN1 or BTN2 was pressed, and sets Flag bits for short or long presses
void buttons(void) {
    Flag &= 0b11110000; // Clears BTN press flag bits
    unsigned int timer = 0; // using int instead of char to avoid rollover. Can only hold btn for 255ms before rollover with uchar and 65535ms with uint 
    if(BTN1) {
        // wait for BTN release
        while(BTN1) {
            timer++;
            __delay_ms(1);
        }  
        // If BTN held for 500ms+
        if(timer < 500) {
            BIT_SET(Flag, short1);
        } else {
            BIT_SET(Flag, long1);
        }
    } else if (BTN2) {
        // wait for BTN release
        while(BTN2) {
            timer++;
            __delay_ms(1);
        } 
        // If BTN held for 500ms+
        if(timer < 500) {
            BIT_SET(Flag, short2);
        } else {
            BIT_SET(Flag, long2);
        }
    }
}

void incDecBCD(void) {
    if(BIT_CHECK(Flag, short1)) {

    } else if (BIT_CHECK(Flag, short2)) {

    }
}