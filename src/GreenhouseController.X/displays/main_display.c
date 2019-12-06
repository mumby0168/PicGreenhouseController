#include "main_display.h"
#include "../drivers/thermometer.h"
#include "../drivers/timing-chip.h"
#include "../drivers/lcd.h"
#include "../fst.h"
#include "../drivers/thermometer.h"

static char* days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
static char* months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static inline void main_display_redraw_dynamic_elements(void)
{
    Timing_ReadCalendar();
    Timing_ReadTime();
    //TIME
    Lcd_SetCursorPosition(1, 1);\
    Lcd_WriteNumber(g_rawClock.hoursTens);\
    Lcd_WriteNumber(g_rawClock.hoursDigits);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteNumber(g_rawClock.minutesTens);
    Lcd_WriteNumber(g_rawClock.minutesDigits);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteNumber(g_rawClock.secondsTens);
    Lcd_WriteNumber(g_rawClock.secondDigits);

    // DAY & DATE
    Lcd_SetCursorPosition(11, 1);
    Lcd_WriteString(days[g_clock.day - 1]);    
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.date);
    
    //Month and year
    Lcd_SetCursorPosition(1, 4);
    Lcd_WriteString(months[g_clock.month - 1]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.year);
    
//    Thermometer_ScratchPad sp;
//    Thermometer_ReadScratchPad(&sp, 2);
//    Thermometer_BcdTemperature tempBcd;
//    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
//    
//    Lcd_SetCursorPosition(1, 2);
//    if (tempBcd.bIsNegative)
//        Lcd_WriteCharacter('-');
//    else
//        Lcd_WriteCharacter(' ');
//    
//    Lcd_WriteCharacter(tempBcd.ubyHundreds + 48);
//    Lcd_WriteCharacter(tempBcd.ubyTens + 48);
//    Lcd_WriteCharacter(tempBcd.ubyUnits + 48);
//    Lcd_WriteCharacter('.');
//    Lcd_WriteCharacter(tempBcd.ubyTenths);
//    Lcd_WriteCharacter(tempBcd.ubyHundredths + 48);
//    Lcd_WriteCharacter(tempBcd.ubyThousandths + 48);
//    Lcd_WriteCharacter(tempBcd.ubyTenThousandths + 48);
}

void Main_Display_Init(void)
{
    Timing_Init();
    Timing_SetTime(10, 11, 50);
    Timing_SetCalendar(1, 2, 3, 4);
    Fst_ClearAction(FST_ACTION_REDRAW_TEMPERATURE);
    Fst_SetAction(FST_ACTION_REDRAW_TEMPERATURE, &main_display_redraw_dynamic_elements);
}

void Main_Display(void)
{
    Timing_ReadCalendar();
    Timing_ReadTime();
    
    //TIME
    Lcd_SetCursorPosition(1, 1);\
    Lcd_WriteNumber(g_rawClock.hoursTens);\
    Lcd_WriteNumber(g_rawClock.hoursDigits);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteNumber(g_rawClock.minutesTens);
    Lcd_WriteNumber(g_rawClock.minutesDigits);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteNumber(g_rawClock.secondsTens);
    Lcd_WriteNumber(g_rawClock.secondDigits);

    // DAY & DATE
    Lcd_SetCursorPosition(11, 1);
    Lcd_WriteString(days[g_clock.day - 1]);    
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.date);
    
    //Month and year
    Lcd_SetCursorPosition(1, 4);
    Lcd_WriteString(months[g_clock.month - 1]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.year);
    
//    Thermometer_ScratchPad sp;
//    Thermometer_ReadScratchPad(&sp, 2);
//    Thermometer_BcdTemperature tempBcd;
//    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
//    
//    Lcd_SetCursorPosition(1, 2);
//    if (tempBcd.bIsNegative)
//        Lcd_WriteCharacter('-');
//    else
//        Lcd_WriteCharacter(' ');
//    
//    Lcd_WriteCharacter(tempBcd.ubyHundreds + 48);
//    Lcd_WriteCharacter(tempBcd.ubyTens + 48);
//    Lcd_WriteCharacter(tempBcd.ubyUnits + 48);
//    Lcd_WriteCharacter('.');
//    Lcd_WriteCharacter(tempBcd.ubyTenths);
//    Lcd_WriteCharacter(tempBcd.ubyHundredths + 48);
//    Lcd_WriteCharacter(tempBcd.ubyThousandths + 48);
//    Lcd_WriteCharacter(tempBcd.ubyTenThousandths + 48);
}