#include "trigger_options.h"
#include "../drivers/lcd.h"
#include "display-std.h"

void Trigger_Options_Display(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteString("1 COLD");

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteString("2 WARM");

    Display_Std_DrawBack();
}