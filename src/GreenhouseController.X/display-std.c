#include <xc.h>
#include "displays/display-std.h"
#include "drivers/lcd.h"

void Display_Std_DrawBack()
{
    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);
}

void Display_Std_DrawSave()
{
    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}