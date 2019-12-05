#include "temp_set.h"
#include "display-std.h"
#include "../drivers/lcd.h"

void Temp_Set_Update(void)
{

    Lcd_SetCursorPosition(1,1);
    if(g_IsDay)
    {
        Lcd_WriteString(g_Day);
    }
    else
    {
        Lcd_WriteString(g_Night);
    }

    Lcd_SetCursorPosition(12, 1);
    if(g_IsHot) 
    {
        Lcd_WriteString(g_Warm);
    }
    else
    {
        Lcd_WriteString(g_Cold);

    }


    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteString("TEMP");


    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}