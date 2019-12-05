#include "clock_settings.h"
#include "display-std.h"
#include "../drivers/lcd.h"


void Clock_Setttings_Display(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteString("1 TIME");

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteString("2 DATE");

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);
}