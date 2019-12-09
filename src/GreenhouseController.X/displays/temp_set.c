#include "temp_set.h"
#include "display-std.h"
#include "../drivers/lcd.h"
#include "../fst.h"
#include "../drivers/eeprom.h"
#include "../libs/Delays.h"


static bool s_IsHot = false;
static bool s_IsDay = false;

static s_ubySelectedDigit = 0;
static uchar s_aubyDigitLimits[5] = { 1, 1, 9, 9, 9 };

static void temp_set_display_render_temp(uchar* pSettingData)
{
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
    Lcd_WriteCharacter('C');
}

static void temp_set_display_select_cold_settings()
{
    s_IsHot = false;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Cold);

    Temp_Set_Display();
}

static void temp_set_display_select_hot_settings()
{
    s_IsHot = true;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Warm);
    
    Temp_Set_Display();
}

static void temp_set_display_load_day_settings()
{
    s_IsDay = true;
}

static void temp_set_display_load_night_settings()
{
    s_IsDay = false;
}

static void temp_set_display_draw_carrot(void)
{
    uchar ubyOffset = 5 + s_ubySelectedDigit;
    Lcd_SetCursorPosition(5, 3);
    Lcd_WriteString("       ");
    
    if (s_ubySelectedDigit == 4)
        ubyOffset++;
    
    Lcd_SetCursorPosition(ubyOffset, 3);
    
    Lcd_WriteCharacter('^');
}

void Temp_Set_Display_Init(void)
{
    Fst_SetAction(FST_ACTION_COLD_SETTINGS, &temp_set_display_select_cold_settings);
    Fst_SetAction(FST_ACTION_HOT_SETTINGS, &temp_set_display_select_hot_settings);
    Fst_SetAction(FST_ACTION_LOAD_DAY_SETTINGS, &temp_set_display_load_day_settings);
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
    
    temp_set_display_render_temp(pSettingData);
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
    
    temp_set_display_render_temp(pSettingData);
}

static void temp_set_display_left_arrow()
{
    if (s_ubySelectedDigit == 0)
        s_ubySelectedDigit = 4;
    else
        s_ubySelectedDigit -= 1;
    
    temp_set_display_draw_carrot();
}

static void temp_set_display_right_arrow()
{
    if (++s_ubySelectedDigit > 4)
        s_ubySelectedDigit = 0;
    
    temp_set_display_draw_carrot();
}

static void temp_set_display_save()
{
    if (!Eeprom_Save())
    {
        Lcd_ClearDisplay();
        Lcd_SetCursorPosition(1, 1);
        Lcd_WriteString("Failed to save settings.");
        DelaySeconds(1);
        Lcd_ClearDisplay();
    }
    else
    {
        Lcd_ClearDisplay();
        Lcd_SetCursorPosition(1, 1);
        Lcd_WriteString("Saved settings.");
        DelaySeconds(1);
        Lcd_ClearDisplay();
    }
}

void Temp_Set_Display(void)
{     
    Eeprom_Load();
    s_ubySelectedDigit = 0;
    
    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &temp_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &temp_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &temp_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &temp_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &temp_set_display_save);
    
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
    
    temp_set_display_render_temp(pSettingData);
    
    Display_Std_DrawBack();
    Display_Std_DrawSave();
    temp_set_display_draw_carrot();
}