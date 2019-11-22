#ifndef LCD_H
#define LCD_H

#include <xc.h>
#include "std.h"

void SetDisplayMode(bool displayOn, bool cursorOn, bool cursorBlink);

void SetDisplayResolution(bool displayTwoLines, bool displayLargeFont);

void ClearDisplay();

void ShiftCursor(bool right);

void ShiftDisplay(bool right);

void MoveCursorToStart(void);

void SetDdramAddress(uchar addr);

void SetCursorPosition(bool secondLine, uchar pos);

void WriteCharacter(char c);

void WriteString(const char* pStr);

void WriteNumber(char num);

void Initialise(void);   

#endif


