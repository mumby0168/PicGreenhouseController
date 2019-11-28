#ifndef LCD_H
#define	LCD_H

typedef unsigned char uchar;
typedef uchar bool;
#define true 1
#define false 0

void Lcd_Init(void);
inline void Lcd_SetDisplayMode(const bool bLcdOn, const bool bCursorOn, const bool bBlinkOn);
void Lcd_SetCursorPosition(uchar ubyPos, uchar ubyLine);
void Lcd_WriteCharacter(const char c);
void Lcd_WriteString(const char* pStr);
inline void Lcd_ClearDisplay();

#endif