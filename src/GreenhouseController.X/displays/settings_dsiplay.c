#include "settings_display.h"
#include "display-std.h"
#include "../drivers/lcd.h"

void Settings_Display(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteString("1  DAY");

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteString("2 NIGHT");

    Lcd_SetCursorPosition(4, 3);
    Lcd_WriteString("3 CLOCK");

    Display_Std_DrawBack();
}