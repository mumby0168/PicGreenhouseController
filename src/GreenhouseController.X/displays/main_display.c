#include "main_display.h"
#include "../drivers/Thermometer.h"
#include "../drivers/timing-chip.h"
#include "../drivers/lcd.h"

static inline void Prepare(void);
static inline void Draw(void);

char* days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
char* months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

void Main_Init(void)
{
    Timing_Init();
}


void Main_Update(void)
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
    Lcd_WriteNumber(g_rawClock.minutesTens);
    Lcd_WriteNumber(g_rawClock.minutesDigits);

    // DAY & DATE
    Lcd_SetCursorPosition(11, 0);
    Lcd_WriteString(days[g_clock.day]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.date);

    // DRAW TEMP
    
    /*

    TEMP CODE HERE

    */

    // MONTH & YEAR
    Lcd_SetCursorPosition(11, 4);
    Lcd_WriteString(months[g_clock.month]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.year);
}


