#include "main_display.h"
#include "../drivers/thermometer.h"
#include "../drivers/timing-chip.h"
#include "../drivers/lcd.h"
#include "../fst.h"
#include "../drivers/thermometer.h"
#include "display-std.h"
#include "../alarm_program.h"

static char* days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
static char* months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static void main_display_render_time_and_temp()
{
    //TIME
    Lcd_SetCursorPosition(1, 1);\
    Lcd_WriteCharacter(g_rawClock.hoursTens + 48);\
    Lcd_WriteCharacter(g_rawClock.hoursDigits + 48);
    Lcd_WriteCharacter(58); // :
    Lcd_WriteCharacter(g_rawClock.minutesTens + 48);
    Lcd_WriteCharacter(g_rawClock.minutesDigits + 48);

    if (Alarm_Program_GetProgram() == ALARM_PROGRAM_DAY)
    {
        Lcd_SetCursorPosition(13, 1);
        Lcd_WriteString(g_Day);
    }
    else
    {
        Lcd_SetCursorPosition(11, 1);
        Lcd_WriteString(g_Night);        
    }
    
    
    // DAY & DATE
    Lcd_SetCursorPosition(1, 4);
    Lcd_WriteString(days[g_clock.day - 1]);    
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.date);
    
    //Month and year
    Lcd_WriteCharacter(' ');
    Lcd_WriteString(months[g_clock.month - 1]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(g_clock.year);
    
    Thermometer_ScratchPad sp;
    if (Thermometer_ReadScratchPad(&sp, 2))
    {
        Lcd_SetCursorPosition(1, 2);
        Lcd_WriteString("Cannot read thermometer.");
        return;
    }
    Thermometer_BcdTemperature tempBcd;
    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
    
    Lcd_SetCursorPosition(1, 2);
    if (tempBcd.bIsNegative)
        Lcd_WriteCharacter('-');
    else
        Lcd_WriteCharacter('+');
    
    Lcd_WriteCharacter(tempBcd.ubyHundreds + 48);
    Lcd_WriteCharacter(tempBcd.ubyTens + 48);
    Lcd_WriteCharacter(tempBcd.ubyUnits + 48);
    Lcd_WriteCharacter('.');
    Lcd_WriteCharacter(tempBcd.ubyTenths + 48);
    Lcd_WriteCharacter(tempBcd.ubyHundredths + 48);
    Lcd_WriteCharacter(tempBcd.ubyThousandths + 48);
    Lcd_WriteCharacter(tempBcd.ubyTenThousandths + 48);
    Lcd_WriteCharacter('C');
}

static inline void main_display_redraw_dynamic_elements(void)
{
    Timing_ReadCalendar();
    Timing_ReadTime();
    
    main_display_render_time_and_temp(); 
}

void Main_Display_Init(void)
{
    Fst_SetAction(FST_ACTION_REDRAW_TEMPERATURE, &main_display_redraw_dynamic_elements);
}

void Main_Display(void)
{
    Timing_ReadCalendar();
    Timing_ReadTime();
    
    main_display_render_time_and_temp();    
}