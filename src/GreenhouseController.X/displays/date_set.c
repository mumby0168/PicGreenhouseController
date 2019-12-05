#include "date_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"


void Date_Set_Display(void)
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteString("DATE SET");

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}