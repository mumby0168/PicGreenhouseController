#include "settings_display.h"
#include "display-std.h"
#include "../drivers/lcd.h"

void Settings_Display(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteCharacter('1');
    Lcd_WriteCharacter(' ');
    Lcd_WriteCharacter(' ');
    Lcd_WriteString(g_Day);

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteCharacter('2');
    Lcd_WriteCharacter(' ');
    Lcd_WriteString(g_Night);

    Lcd_SetCursorPosition(4, 3);
    Lcd_WriteCharacter('3');
    Lcd_WriteCharacter(' ');
    Lcd_WriteString("CLOCK");

    Lcd_SetCursorPosition(1,4);
    Lcd_WriteString(g_Back);   
}