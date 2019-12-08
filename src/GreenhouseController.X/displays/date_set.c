#include "date_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"
#include "../fst.h"
#include "../drivers/timing-chip.h"
#include "../libs/Delays.h"

typedef struct _date_set
{
    uchar day;
    uchar month;
    uchar year;
} DateSet;

static DateSet s_DateSet;
static uchar s_ubyDigitPosition = 0;
static uchar s_aubyDateLimits[6] = { 31, 12, 99 };

static void date_set_display_render_date()
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteNumber(s_DateSet.day);    
    Lcd_WriteCharacter('/');
    
    Lcd_WriteNumber(s_DateSet.month);   
    Lcd_WriteCharacter('/');
    
    Lcd_WriteNumber(s_DateSet.year);
}

static void date_set_display_up_arrow(void)
{
    uchar* pDigit = &s_DateSet;
    pDigit += s_ubyDigitPosition;
    
    if (++(*pDigit) > s_aubyDateLimits[s_ubyDigitPosition])
    {
        *pDigit = 0;
    }
    
    date_set_display_render_date();
}

static void date_set_display_down_arrow(void)
{
    uchar* pDigit = &s_DateSet;
    pDigit += s_ubyDigitPosition;
    
    if ((*pDigit) == 1)
    {
        *pDigit = s_aubyDateLimits[s_ubyDigitPosition];
    }
    else
    {
        (*pDigit)--;
    }
    
    date_set_display_render_date();
}

static void date_set_display_left_arrow(void)
{
    if (s_ubyDigitPosition == 0)
    {
        s_ubyDigitPosition = 5;
    }
    else
    {
        s_ubyDigitPosition--;
    }
    
//    Lcd_SetCursorPosition(5 + s_ubyDigitPosition, 3);
//    Lcd_WriteCharacter('^');
}

static void date_set_display_right_arrow(void)
{
    if (++s_ubyDigitPosition > 5)
    {
        s_ubyDigitPosition = 0;
    }
    
//    Lcd_SetCursorPosition(5 + s_ubyDigitPosition, 3);
//    Lcd_WriteCharacter('^');
}

static void date_set_display_save(void)
{
    Timing_SetCalendar(Timing_GetDayFromDate(s_DateSet.day, s_DateSet.month, s_DateSet.year), s_DateSet.day, s_DateSet.month, s_DateSet.year);
    
    Lcd_ClearDisplay();
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteString("Saved settings.");
    DelaySeconds(1);
    Lcd_ClearDisplay();
}

void Date_Set_Display(void)
{
    Timing_ReadTime();
    uchar* pClock = &g_clock;
    memcpy(&s_DateSet, pClock + (sizeof(Clock) - sizeof(DateSet)), sizeof(DateSet));

    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &date_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &date_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &date_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &date_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &date_set_display_save);
    
    date_set_display_render_date();
    
    Lcd_SetCursorPosition(1,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}