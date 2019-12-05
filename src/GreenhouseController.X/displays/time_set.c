#include "time_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"


void Time_Set_Update(void)
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteString("TIME SET");


    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}