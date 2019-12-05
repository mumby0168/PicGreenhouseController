#include "settings_display.h"
#include "display-std.h"
#include "../drivers/lcd.h"


static inline void Prepare(void);
static inline void Draw(void);

void Init(void)
{

}


void Update(void)
{

}


static inline void Prepare(void)
{
    //Maybe Clear Display?
}

static inline void Draw(void)
{
    Lcd_SetCursorPosition(4, 1);
    Lcd_WriteCharacter('1');
    Lcd_WriteCharacter('/0');
    Lcd_WriteCharacter('/0');
    Lcd_WriteString(g_Day);

    Lcd_SetCursorPosition(4, 2);
    Lcd_WriteCharacter('2');
    Lcd_WriteCharacter('/0');
    Lcd_WriteString(g_Night);

    Lcd_SetCursorPosition(4, 3);
    Lcd_WriteCharacter('3');
    Lcd_WriteCharacter('/0');
    Lcd_WriteString("CLOCK");

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);
}
