#include "time_set.h"
#include "../drivers/lcd.h"
#include "display-std.h"
#include "../fst.h"
#include "../drivers/timing-chip.h"

static void display_time(void);


typedef struct 
{
    uchar secondsTens;
    uchar secondsDigits;
    uchar minutesTens;
    uchar minutesDigits;
    uchar hoursTens;
    uchar hoursDigits;
} ClockSettings;



ClockSettings g_clockSet;

static char g_TimeMemCounter = 0;
static char g_TimeLimits[5] = { 6, 9, 6, 9, 2, 9 };
#define CLOCK_SETTINGS_SIZE 5


static void time_set_display_down_arrow()
{
    uchar* pClockSettings = &g_clockSet;

    pClockSettings += g_TimeMemCounter;

    uchar val = *pClockSettings;

    val--;
    if(val < 0)
    {
        val = g_TimeLimits[g_TimeMemCounter];
    }

    display_time();
}

static void time_set_display_left_arrow()
{
    g_TimeMemCounter++;
    if(g_TimeMemCounter > CLOCK_SETTINGS_SIZE)
        g_TimeMemCounter = 0;    

    display_time();
}

static void time_set_display_right_arrow()
{
    g_TimeMemCounter--;
    if(g_TimeMemCounter < 0)
        g_TimeMemCounter = CLOCK_SETTINGS_SIZE;

    display_time();
}

static void time_set_display_save_button()
{

    uchar hours = (g_clockSet.hoursTens * 10) + g_clockSet.hoursDigits;
    uchar mins = (g_clockSet.minutesTens * 10) + g_clockSet.minutesDigits;
    uchar secs = (g_clockSet.secondsTens * 10) + g_clockSet.secondsDigits;

    Timing_SetTime(hours, mins, secs);
}

static void time_set_display_up_arrow()
{
    uchar* pClockSettings = &g_clockSet;

    pClockSettings += g_TimeMemCounter;

    uchar val = *pClockSettings;

    val++;
    if(val < g_TimeLimits[g_TimeMemCounter])
    {
        val = 0;
    }

    display_time();
}

void Time_Set_Display_Init(void)
{

    g_clockSet = *(ClockSettings*)&g_rawClock; // Init the clock the current time

    Fst_ClearAction(FST_ACTION_HANDLE_UP_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &time_set_display_up_arrow);

    Fst_ClearAction(FST_ACTION_HANDLE_DOWN_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &time_set_display_down_arrow);

    Fst_ClearAction(FST_ACTION_HANDLE_RIGHT_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &time_set_display_right_arrow);

    Fst_ClearAction(FST_ACTION_HANDLE_LEFT_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &time_set_display_left_arrow);

    //TODO: add a handle action to save the time.
}

void Time_Set_Display(void)
{    
    display_time();

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}


static void display_time()
{
    Lcd_SetCursorPosition(5, 2);

    Lcd_WriteNumber(g_clockSet.hoursTens);
    Lcd_WriteNumber(g_clockSet.hoursDigits);
    Lcd_WriteCharacter(58);

    Lcd_WriteNumber(g_clockSet.minutesTens);
    Lcd_WriteNumber(g_clockSet.minutesDigits);
    Lcd_WriteCharacter(58);

    Lcd_WriteNumber(g_clockSet.secondsTens);
    Lcd_WriteNumber(g_clockSet.secondDigits);
}