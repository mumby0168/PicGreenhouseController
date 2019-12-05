#include "temp_set.h"
#include "display-std.h"
#include "../drivers/lcd.h"
#include "../fst.h"


static bool s_IsHot = false;
static bool s_IsDay = false;

static void temp_set_display_select_cold_settings()
{
    s_IsHot = false;
}

static void temp_set_display_select_hot_settings()
{
    s_IsHot = true;
}

static void temp_set_display_load_day_settings()
{
    s_IsDay = true;
}

static void temp_set_display_load_night_settings()
{
    s_IsDay = false;
}

void Temp_Set_Display_Init(void)
{
    Fst_ClearAction(FST_ACTION_COLD_SETTINGS);
    Fst_SetAction(FST_ACTION_COLD_SETTINGS, &temp_set_display_select_cold_settings);
    
    Fst_ClearAction(FST_ACTION_HOT_SETTINGS);
    Fst_SetAction(FST_ACTION_HOT_SETTINGS, &temp_set_display_select_hot_settings);
    
    Fst_ClearAction(FST_ACTION_LOAD_DAY_SETTINGS);
    Fst_SetAction(FST_ACTION_LOAD_DAY_SETTINGS, &temp_set_display_load_day_settings);
    
    Fst_ClearAction(FST_ACTION_LOAD_NIGHT_SETTINGS);
    Fst_SetAction(FST_ACTION_LOAD_NIGHT_SETTINGS, &temp_set_display_load_night_settings);
}

void Temp_Set_Display(void)
{     
    Lcd_SetCursorPosition(1,1);
    if(s_IsDay)
    {
        Lcd_WriteString(g_Day);
    }
    else
    {
        Lcd_WriteString(g_Night);
    }

    Lcd_SetCursorPosition(12, 1);
    if(s_IsHot) 
    {
        Lcd_WriteString(g_Warm);
    }
    else
    {
        Lcd_WriteString(g_Cold);

    }

    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteString("TEMP");

    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}