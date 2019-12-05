#include "trigger-opts.h"
#include "../drivers/lcd.h"
#include "display-std.h"


void Trigger_Options_Update(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteString(g_Cold);

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteString(g_Warm);

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);
}