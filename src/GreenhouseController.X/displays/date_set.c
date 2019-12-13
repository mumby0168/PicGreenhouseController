#include "date_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"
#include "../fst.h"
#include "../drivers/timing-chip.h"
#include "../libs/Delays.h"

typedef struct _date_set
{
    uchar date;
    uchar month;
    uchar year;
} DateSet;

static DateSet s_DateSet;
static uchar s_ubyDigitPosition = 0;
static uchar s_aubyDateLimits[3] = { 31, 12, 99 };

static void date_set_display_draw_carrot(void)
{
    Lcd_SetCursorPosition(5, 3);
    Lcd_WriteString("        ");
    
    uchar ubyOffset = 5 + (s_ubyDigitPosition * 2);
    ubyOffset += s_ubyDigitPosition;
    
    Lcd_SetCursorPosition(ubyOffset, 3);
    Lcd_WriteString("^^");
}

static void date_set_display_render_date()
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteNumber(s_DateSet.date);    
    Lcd_WriteCharacter('/');
    
    Lcd_WriteNumber(s_DateSet.month);   
    Lcd_WriteCharacter('/');
    
    Lcd_WriteNumber(s_DateSet.year);
}

static void date_set_update_limits()
{
    //modifies limits based on leap year or not.
    s_aubyDateLimits[0] = g_aubyDaysInMonths[s_DateSet.month - 1];
    if (Timing_IsLeapYear(s_DateSet.year) && s_DateSet.month == 2)
        s_aubyDateLimits[0] += 1;
    
    //resets if limit reached
    if (s_DateSet.date > s_aubyDateLimits[0])
        s_DateSet.date = s_aubyDateLimits[0];
    
    //re-renders
    date_set_display_render_date();
}

static void date_set_display_up_arrow(void)
{
    uchar* pDigit = &s_DateSet;
    pDigit += s_ubyDigitPosition;
    
    //increments the value of the pntr depending on number modifying.
    if (++(*pDigit) > s_aubyDateLimits[s_ubyDigitPosition])
    {
        *pDigit = 1;
    }
    
    date_set_display_render_date();
    date_set_update_limits();
}

static void date_set_display_down_arrow(void)
{
    uchar* pDigit = &s_DateSet;
    pDigit += s_ubyDigitPosition;
    
    //resets value to high limit if lower reached.
    if ((*pDigit) == 1)
    {
        *pDigit = s_aubyDateLimits[s_ubyDigitPosition];
    }
    else
    {
        (*pDigit)--;
    }
    
    date_set_display_render_date();
    date_set_update_limits();
}

static void date_set_display_left_arrow(void)
{
    if (s_ubyDigitPosition == 0)
    {
        s_ubyDigitPosition = 2;
    }
    else
    {
        s_ubyDigitPosition--;
    }
    
    date_set_update_limits();
    date_set_display_draw_carrot();
}

static void date_set_display_right_arrow(void)
{
    if (++s_ubyDigitPosition > 2)
    {
        s_ubyDigitPosition = 0;
    }
    
    date_set_update_limits();
    date_set_display_draw_carrot();
}

static void date_set_display_save(void)
{
    Timing_SetCalendar(Timing_GetDayFromDate(s_DateSet.date, s_DateSet.month, s_DateSet.year), s_DateSet.date, s_DateSet.month, s_DateSet.year);
    
    Lcd_ClearDisplay();
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteString("Saved settings.");
    DelaySeconds(1);
    Lcd_ClearDisplay();
}

void Date_Set_Display(void)
{
    s_ubyDigitPosition = 0;
    Timing_ReadTime();
    Timing_Clock clock;
    Timing_ReadClock(&clock);
    uchar* pClock = &clock;
    memcpy(&s_DateSet, pClock + (sizeof(Timing_Clock) - sizeof(DateSet)), sizeof(DateSet));

    //sets up action hanlders for this display.
    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &date_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &date_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &date_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &date_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &date_set_display_save);
    
    date_set_display_render_date();
    
    Display_Std_DrawBack();
    Display_Std_DrawSave();
    date_set_display_draw_carrot();
}