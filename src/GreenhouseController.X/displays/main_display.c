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
static bool s_bPrevDrawHadError = false;

static void main_display_render_time_and_temp()
{
    
    RawClock rawClock;
    Timing_ReadRawClock(&rawClock);
    Clock clock;
    Timing_ReadClock(&clock);
    
    //TIME
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteCharacter(rawClock.ucHoursTens + 48);
    Lcd_WriteCharacter(rawClock.ucHoursDigits + 48);
    Lcd_WriteCharacter(':'); 
    Lcd_WriteCharacter(rawClock.ucMinutesTens + 48);
    Lcd_WriteCharacter(rawClock.ucMinutesDigits + 48);
    Lcd_WriteCharacter(':'); 
    Lcd_WriteCharacter(rawClock.ucSecondsTens + 48);
    Lcd_WriteCharacter(rawClock.ucSecondsDigits + 48);
    
    if (Alarm_Program_GetProgram() == ALARM_PROGRAM_DAY)
    {
        Lcd_SetCursorPosition(12, 1);
        Lcd_WriteString("  DAY");
    }
    else
    {
        Lcd_SetCursorPosition(12, 1);
        Lcd_WriteString("NIGHT");        
    }
    
    // DAY & DATE
    Lcd_SetCursorPosition(2, 4);
    Lcd_WriteString(days[clock.ucDay - 1]);    
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(clock.ucDate);
    
    //Month and year
    Lcd_WriteCharacter(' ');
    Lcd_WriteCharacter(' ');
    Lcd_WriteString(months[clock.ucMonth - 1]);
    Lcd_WriteCharacter(' ');
    Lcd_WriteNumber(clock.ucYear);
       
    if (Alarm_Program_IsCooling())
    {
        Lcd_SetCursorPosition(5, 3);
        Lcd_WriteString("COOLING"); 
    }
    else if (Alarm_Program_IsHeating())
    {
        Lcd_SetCursorPosition(5, 3);
        Lcd_WriteString("HEATING");
    }
    else
    {
        Lcd_SetCursorPosition(5, 3);
        Lcd_WriteString("       ");
    }
    
    Thermometer_ScratchPad sp;
    if (Thermometer_ReadScratchPad(&sp, 2))
    {
        Lcd_SetCursorPosition(1, 2);
        Lcd_WriteString("Cannot read thermometer.");
        s_bPrevDrawHadError = true;
        return;
    }
    else if (s_bPrevDrawHadError)
    {
        Lcd_SetCursorPosition(1, 2);
        Lcd_WriteString("                        ");
    }
    Thermometer_BcdTemperature tempBcd;
    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
    
    Lcd_SetCursorPosition(5, 2);
    if (tempBcd.bIsNegative)
        Lcd_WriteCharacter('-');
    else
        Lcd_WriteCharacter('+');
    
    Lcd_WriteCharacter(tempBcd.ubyHundreds + 48);
    Lcd_WriteCharacter(tempBcd.ubyTens + 48);
    Lcd_WriteCharacter(tempBcd.ubyUnits + 48);
    Lcd_WriteCharacter('.');
    Lcd_WriteCharacter(tempBcd.ubyTenths + 48);
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