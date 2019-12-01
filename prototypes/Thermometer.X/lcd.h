#ifndef LCD_H
#define	LCD_H

#include "std.h"

void Lcd_Init(void);
inline void Lcd_SetDisplayMode(const bool bLcdOn, const bool bCursorOn, const bool bBlinkOn);
void Lcd_SetCursorPosition(uchar ubyPos, uchar ubyLine);
void Lcd_WriteCharacter(const char c);
void Lcd_WriteString(const char* pStr);
void Lcd_RecallLastPosition();
inline void Lcd_ClearDisplay();

#endif