#include "../libs/std.h"
#include "time_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"
#include "../drivers/timing-chip.h"
#include "../fst.h"
#include "../libs/Delays.h"

static RawClock s_TimeSet;
static uchar s_ubyDigitPosition = 0;
static uchar s_aubyTimeLimits[6] = { 2, 9, 5, 9, 5, 9 };

static void time_set_update_time_limits()
{
    if (s_TimeSet.hoursTens == 2)
    {
        s_aubyTimeLimits[1] = 4;
    }
    else
    {
        s_aubyTimeLimits[1] = 9;
    }
}

static void time_set_display_render_time(void)
{
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteCharacter(s_TimeSet.hoursTens + 48);
    Lcd_WriteCharacter(s_TimeSet.hoursDigits + 48);
    
    Lcd_WriteCharacter(':');
    Lcd_WriteCharacter(s_TimeSet.minutesTens + 48);
    Lcd_WriteCharacter(s_TimeSet.minutesDigits + 48);
    
    Lcd_WriteCharacter(':');
    Lcd_WriteCharacter(s_TimeSet.secondsTens + 48);
    Lcd_WriteCharacter(s_TimeSet.secondsDigits + 48);
}

static void time_set_display_up_arrow(void)
{
    time_set_update_time_limits();
    uchar* pDigit = &s_TimeSet;
    pDigit += s_ubyDigitPosition;
    
    if (++(*pDigit) > s_aubyTimeLimits[s_ubyDigitPosition])
    {
        *pDigit = 0;
    }
    
    time_set_display_render_time();
}

static void time_set_display_down_arrow(void)
{
    time_set_update_time_limits();
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
    
//    Lcd_SetCursorPosition(5 + s_ubyDigitPosition, 3);
//    Lcd_WriteCharacter('^');
}

static void time_set_display_right_arrow(void)
{
    if (++s_ubyDigitPosition > 5)
    {
        s_ubyDigitPosition = 0;
    }
    
//    Lcd_SetCursorPosition(5 + s_ubyDigitPosition, 3);
//    Lcd_WriteCharacter('^');
}

static void time_set_display_save(void)
{
    Timing_SetTime((s_TimeSet.hoursTens * 10) + s_TimeSet.hoursDigits,
                   (s_TimeSet.minutesTens * 10) + s_TimeSet.minutesDigits,
                   (s_TimeSet.secondsTens * 10) + s_TimeSet.secondsDigits);
    
    Lcd_ClearDisplay();
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteString("Saved settings.");
    DelaySeconds(1);
    Lcd_ClearDisplay();
}

void Time_Set_Display(void)
{
    Timing_ReadTime();
    memcpy(&s_TimeSet, &g_rawClock, sizeof(RawClock));

    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &time_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &time_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &time_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &time_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &time_set_display_save);
    
    time_set_display_render_time();
    
    Display_Std_DrawBack();
    Display_Std_DrawSave();
}