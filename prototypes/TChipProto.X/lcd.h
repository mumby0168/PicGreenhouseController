#ifndef LCD_H
#define LCD_H

#include <xc.h>
typedef unsigned char uchar;
typedef uchar bool;

void delay() { int i;for(i=0;i<5000;i++); }

inline void SetDisplayMode(bool displayOn, bool cursorOn, bool cursorBlink);

inline void SetDisplayResolution(bool displayTwoLines, bool displayLargeFont);

inline void ClearDisplay();

void ShiftCursor(bool right);

void ShiftDisplay(bool right);

inline void MoveCursorToStart();

void SetDdramAddress(uchar addr);

inline void SetCursorPosition(bool secondLine, uchar pos);

inline void WriteCharacter(char c);

inline void Initialise();   

#endif


