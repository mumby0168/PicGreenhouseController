#include "clock_settings.h"
#include "display-std.h"
#include "../drivers/lcd.h"


void Clock_Setttings_Display(void)
{
    Lcd_SetCursorPosition(5, 1);
    Lcd_WriteString("1 TIME");

    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteString("2 DATE");

    Display_Std_DrawBack();
}