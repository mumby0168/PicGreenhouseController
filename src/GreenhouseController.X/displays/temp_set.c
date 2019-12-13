#include "temp_set.h"
#include "display-std.h"
#include "../drivers/lcd.h"
#include "../fst.h"
#include "../drivers/eeprom.h"
#include "../libs/Delays.h"
#include "../drivers/thermometer.h"
#include "time_set.h"


static bool s_bIsHot = false;
static bool s_bIsDay = false;

static s_ubySelectedDigit = 0;
static uchar s_aubyDigitLimits[5] = { 1, 1, 9, 9, 9 };
static Eeprom_Eeprom s_eeprom;

//takes a pointer to the part of the settings to display
//dereference it and then increment it in each part to get the next part of the BCD
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

static void temp_set_display_select_cold_settings(void)
{
    s_bIsHot = false;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Cold);

    Temp_Set_Display();
}

static void temp_set_display_select_hot_settings()
{
    s_bIsHot = true;
    Lcd_SetCursorPosition(12, 1);
    Lcd_WriteString(g_Warm);
    
    Temp_Set_Display();
}

static void temp_set_display_load_day_settings()
{
    s_bIsDay = true;
}

static void temp_set_display_load_night_settings()
{
    s_bIsDay = false;
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
    //Register the action handlers for loading the settings
    Fst_SetAction(FST_ACTION_COLD_SETTINGS, &temp_set_display_select_cold_settings);
    Fst_SetAction(FST_ACTION_HOT_SETTINGS, &temp_set_display_select_hot_settings);
    Fst_SetAction(FST_ACTION_LOAD_DAY_SETTINGS, &temp_set_display_load_day_settings);
    Fst_SetAction(FST_ACTION_LOAD_NIGHT_SETTINGS, &temp_set_display_load_night_settings);
}

static void temp_set_display_up_arrow()
{
    Lcd_SetCursorPosition(1, 1);
    uchar* pSettingData = &s_eeprom; //take a pointer to the alarm settings
    
    if (!s_bIsDay) //if it is day sit over the daytime settings
        pSettingData += sizeof(Eeprom_AlarmSettings);
    
    if (!s_bIsHot) //if it is the cold setting skip over the warm settings
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES;   
    
    //increment the pointer by number of the current digit the user is editing
    pSettingData += s_ubySelectedDigit;
    
    //increment the value which the pointer is stored in, if it is greater then the limit set it to zero
    if (++(*pSettingData) > s_aubyDigitLimits[s_ubySelectedDigit])
        *pSettingData = 0;
    
    //decrement the pointer so it is pointing back to the first digit
    pSettingData -= s_ubySelectedDigit;

    //redraw the tempreature
    temp_set_display_render_temp(pSettingData);
}
    
static void temp_set_display_down_arrow()
{
    Lcd_SetCursorPosition(1, 1);
    uchar* pSettingData = &s_eeprom; //take a pointer to the alarm settings
    
    if (!s_bIsDay) //if it is day sit over the daytime settings
        pSettingData += sizeof(Eeprom_AlarmSettings);
    
    if (!s_bIsHot) //if it is the cold setting skip over the warm settings
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES;   
    
    //increment the pointer by number of the current digit the user is edit
    pSettingData += s_ubySelectedDigit;
    
    //if the current value is zero go back to the limit otherwise decrement the value thats pointed to
    if (*pSettingData == 0)
        *pSettingData = s_aubyDigitLimits[s_ubySelectedDigit];
    else
        (*pSettingData)--;
    
    //decrement the pointer so it is pointing back to the first digit
    pSettingData -= s_ubySelectedDigit;
    
    //redraw the tempreature
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
    Eeprom_SetEeprom(&s_eeprom);
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
    
    //Register the actions delegates to the action each time as other displays will overwrite these
    Fst_SetAction(FST_ACTION_HANDLE_UP_BUTTON, &temp_set_display_up_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_DOWN_BUTTON, &temp_set_display_down_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_LEFT_BUTTON, &temp_set_display_left_arrow);
    Fst_SetAction(FST_ACTION_HANDLE_RIGHT_BUTTON, &temp_set_display_right_arrow);
    Fst_SetAction(FST_ACTION_SAVE, &temp_set_display_save);
    
    Eeprom_GetEeprom(&s_eeprom);
    uchar* pSettingData = &s_eeprom; //take a pointer to the settings
    
    Lcd_SetCursorPosition(1,1);
    if(s_bIsDay)
    {
        Lcd_WriteString(g_Day);
    }
    else
    {
        Lcd_WriteString(g_Night);
        pSettingData += sizeof(Eeprom_AlarmSettings);//skip over the day time settings
    }

    Lcd_SetCursorPosition(12, 1);
    if(s_bIsHot) 
    {
        Lcd_WriteString(g_Warm);
    }
    else
    {
        Lcd_WriteString(g_Cold);
        pSettingData += EEPROM_WARM_COLD_ALARM_BYTES; //skip over the warm settings
    }

    Lcd_SetCursorPosition(5, 2);
    
    temp_set_display_render_temp(pSettingData); //render the temperature that is been pointed to...
    
    Display_Std_DrawBack();
    Display_Std_DrawSave();
    temp_set_display_draw_carrot();
}