#include "main_display.h"
#include "../drivers/Thermometer.h"
#include "../drivers/timing-chip.h"
#include "../drivers/lcd.h"

static inline void Prepare(void);
static inline void Draw(void);

char* days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
char* months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

void Init(void)
{
    Timing_Init();
}


void Update(void)
{
    Prepare();
    Draw();
}



static inline void Prepare(void)
{
    Timing_ReadCalendar();
    Timing_ReadTime();
}

static inline void Draw(void)
{
    //TIME
    Lcd_SetCursorPosition(0, 0);
    Lcd_WriteNumber(g_rawClock.hoursTens);
    Lcd_WriteNumber(g_rawClock.hoursDigits);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteNumber(g_rawClock.secondsTens);
    Lcd_WriteNumber(g_rawClock.secondDigits);

    // DAY & DATE
    Lcd_SetCursorPosition(11, 0);
    Lcd_WriteString(days[g_clock.day]);
    Lcd_WriteCharacter('/0');
    Lcd_WriteNumber(g_clock.date);

    // DRAW TEMP

    // MONTH & YEAR
    Lcd_SetCursorPosition(11, 4);
    Lcd_WriteString(months[g_clock.month]);
    Lcd_WriteCharacter('/0');
    Lcd_WriteNumber(g_clock.year);
}


