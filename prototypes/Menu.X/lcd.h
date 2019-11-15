#ifndef LCD_H
#define LCD_H

#include <xc.h>
#define true 1
#define false 0

typedef unsigned char uchar;
typedef uchar bool;

void delay();

void SetDisplayMode(bool displayOn, bool cursorOn, bool cursorBlink);

void SetDisplayResolution(bool displayTwoLines, bool displayLargeFont);

void ClearDisplay();

void ShiftCursor(bool right);

void ShiftDisplay(bool right);

void MoveCursorToStart();

void SetDdramAddress(uchar addr);

void SetCursorPosition(bool secondLine, uchar pos);

void WriteCharacter(char c);
void WriteNumber(char num);
void Initialise();   

#endif


