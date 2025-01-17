#include "temp_set.h"
#include "display-std.h"
#include "../drivers/lcd.h"
#include "../fst.h"
#include "../drivers/eeprom.h"


static bool s_IsHot = false;
static bool s_IsDay = false;

static s_ubySelectedDigit = 0;
static uchar s_aubyDigitLimits[5] = { 1, 1, 9, 9, 9 };


static void temp_set_display_select_cold_settings()
{
    s_IsHot = false;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Cold);
}

static void temp_set_display_select_hot_settings()
{
    s_IsHot = true;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Warm);
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

static void temp_set_display_up_arrow()
{
    Lcd_SetCursorPosition(1, 1);
    uchar* pSettingData = &Eeprom_Settings;
    
    if (!s_IsDay)
        pSettingData += sizeof(Eeprom_AlarmSettings);
    
    if (!s_IsHot)
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES;   
    
    pSettingData += s_ubySelectedDigit;
    
    if (++(*pSettingData) > s_aubyDigitLimits[s_ubySelectedDigit])
        *pSettingData = 0;
    
    pSettingData -= s_ubySelectedDigit;
    
    Lcd_SetCursorPosition(5, 2);
    if (*pSettingData)
        Lcd_WriteCharacter('-');
    else
        Lcd_WriteCharacter('+');

    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    Lcd_WriteCharacter('.');
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
}
    
static void temp_set_display_down_arrow()
{
    Lcd_SetCursorPosition(1, 1);
    uchar* pSettingData = &Eeprom_Settings;
    
    if (!s_IsDay)
        pSettingData += sizeof(Eeprom_AlarmSettings);
    
    if (!s_IsHot)
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES;   
    
    pSettingData += s_ubySelectedDigit;
    
    if (*pSettingData == 0)
        *pSettingData = s_aubyDigitLimits[s_ubySelectedDigit];
    else
        (*pSettingData)--;
    
    
    pSettingData -= s_ubySelectedDigit;
    
    Lcd_SetCursorPosition(5, 2);
    if (*pSettingData)
        Lcd_WriteCharacter('-');
    else
        Lcd_WriteCharacter('+');

    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    Lcd_WriteCharacter('.');
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
}

void temp_set_display_left_arrow()
{
    if (s_ubySelectedDigit == 0)
        s_ubySelectedDigit = 4;
    else
        s_ubySelectedDigit -= 1;
}

void temp_set_display_right_arrow()
{
    if (++s_ubySelectedDigit > 4)
        s_ubySelectedDigit = 0;
}

void Temp_Set_Display(void)
{     
    s_ubySelectedDigit = 0;
    Fst_ClearAction(FST_ACTION_HANDLE_UP_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &temp_set_display_up_arrow);
    
    Fst_ClearAction(FST_ACTION_HANDLE_DOWN_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &temp_set_display_down_arrow);
    
    Fst_ClearAction(FST_ACTION_HANDLE_LEFT_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &temp_set_display_left_arrow);
    
    Fst_ClearAction(FST_ACTION_HANDLE_RIGHT_BUTTON);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &temp_set_display_right_arrow);
    
    uchar* pSettingData = &Eeprom_Settings;
    
    Lcd_SetCursorPosition(1,1);
    if(s_IsDay)
    {
        Lcd_WriteString(g_Day);
    }
    else
    {
        Lcd_WriteString(g_Night);
        pSettingData += sizeof(Eeprom_AlarmSettings);
    }

    Lcd_SetCursorPosition(12, 1);
    if(s_IsHot) 
    {
        Lcd_WriteString(g_Warm);
    }
    else
    {
        Lcd_WriteString(g_Cold);
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES;
    }

    Lcd_SetCursorPosition(5, 2);
    if (*pSettingData)
        Lcd_WriteCharacter('-');
    else
        Lcd_WriteCharacter('+');
        
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    Lcd_WriteCharacter('.');
    pSettingData++;
    Lcd_WriteCharacter(*pSettingData + 48);
    
    Lcd_SetCursorPosition(0,4);
    Lcd_WriteString(g_Back);

    Lcd_SetCursorPosition(12,4);
    Lcd_WriteString(g_Save);
}