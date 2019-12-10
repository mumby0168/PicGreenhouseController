#include "../libs/std.h"
#include "time_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"
#include "../drivers/timing-chip.h"
#include "../fst.h"
#include "../libs/Delays.h"

static Timing_RawClock s_TimeSet;
static uchar s_ubyDigitPosition = 0;
static uchar s_aubyTimeLimits[6] = { 2, 9, 5, 9, 5, 9 };

static void time_set_display_draw_carrot(void)
{
    Lcd_SetCursorPosition(5, 3);
    Lcd_WriteString("        ");
    
    uchar ubyOffset = 5 + s_ubyDigitPosition;
    ubyOffset += s_ubyDigitPosition / 2;
    
    Lcd_SetCursorPosition(ubyOffset, 3);
    Lcd_WriteCharacter('^');
}

static void time_set_display_render_time(void)
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteCharacter(s_TimeSet.ucHoursTens + 48);
    Lcd_WriteCharacter(s_TimeSet.ucHoursDigits + 48);
    
    Lcd_WriteCharacter(':');
    Lcd_WriteCharacter(s_TimeSet.ucMinutesTens + 48);
    Lcd_WriteCharacter(s_TimeSet.ucMinutesDigits + 48);
    
    Lcd_WriteCharacter(':');
    Lcd_WriteCharacter(s_TimeSet.ucSecondsTens + 48);
    Lcd_WriteCharacter(s_TimeSet.ucSecondsDigits + 48);
}

static void time_set_update_time_limits()
{
    if (s_TimeSet.ucHoursTens == 2)
    {
        s_aubyTimeLimits[1] = 3;
        if (s_TimeSet.ucHoursDigits > s_aubyTimeLimits[1])
        {
            s_TimeSet.ucHoursDigits = s_aubyTimeLimits[1];
        }
    }
    else
    {
        s_aubyTimeLimits[1] = 9;
    }
    
    time_set_display_render_time();
}

static void time_set_display_up_arrow(void)
{
    uchar* pDigit = &s_TimeSet;
    pDigit += s_ubyDigitPosition;
    
    if (++(*pDigit) > s_aubyTimeLimits[s_ubyDigitPosition])
    {
        *pDigit = 0;
    }
    
    time_set_display_render_time();
    time_set_update_time_limits();
}

static void time_set_display_down_arrow(void)
{
    uchar* pDigit = &s_TimeSet;
    pDigit += s_ubyDigitPosition;
    
    if ((*pDigit) == 0)
    {
        *pDigit = s_aubyTimeLimits[s_ubyDigitPosition];
    }
    else
    {
        (*pDigit)--;
    }
    
    time_set_display_render_time();
    time_set_update_time_limits();
}

static void time_set_display_left_arrow(void)
{
    if (s_ubyDigitPosition == 0)
    {
        s_ubyDigitPosition = 5;
    }
    else
    {
        s_ubyDigitPosition--;
    }
    
    time_set_update_time_limits();
    time_set_display_draw_carrot();
}

static void time_set_display_right_arrow(void)
{
    if (++s_ubyDigitPosition > 5)
    {
        s_ubyDigitPosition = 0;
    }
    
    time_set_update_time_limits();
    time_set_display_draw_carrot();
}

static void time_set_display_save(void)
{
    Timing_SetTime((s_TimeSet.ucHoursTens * 10) + s_TimeSet.ucHoursDigits,
                   (s_TimeSet.ucMinutesTens * 10) + s_TimeSet.ucMinutesDigits,
                   (s_TimeSet.ucSecondsTens * 10) + s_TimeSet.ucSecondsDigits);
    
    Lcd_ClearDisplay();
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteString("Saved settings.");
    DelaySeconds(1);
    Lcd_ClearDisplay();
}

void Time_Set_Display(void)
{
    s_ubyDigitPosition = 0;
    Timing_ReadTime();
    Timing_RawClock rawClock;
    Timing_ReadRawClock(&rawClock);
    memcpy(&s_TimeSet, &rawClock, sizeof(Timing_RawClock));

    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &time_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &time_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &time_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &time_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &time_set_display_save);
    
    time_set_display_render_time();
    
    Display_Std_DrawBack();
    Display_Std_DrawSave();
    time_set_display_draw_carrot();
}