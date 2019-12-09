#include <xc.h>
#include "alarm_program.h"
#include "drivers/timing-chip.h"
#include "drivers/thermometer.h"
#include "drivers/eeprom.h"
#include "drivers/lcd.h"

#define CONVERT_HOURS_AND_MINS_TO_MINS(h, m) (h * 60) + m

static Alarm_Program_Programs s_CurrentProgram = ALARM_PROGRAM_DAY;
static const unsigned short s_usDayProgramStartTimeInMins = CONVERT_HOURS_AND_MINS_TO_MINS(6, 30);
static const unsigned short s_usNightProgramStartTimeInMins = CONVERT_HOURS_AND_MINS_TO_MINS(18, 30);

void Alarm_Program_Init(void)
{
    TRISE = 0b000;
    PORTE = 0b000;    
    
    Alarm_Program_Update();
}

static int alarm_program_get_temp_as_short(const Thermometer_BcdTemperature* const pBcd)
{
    if (pBcd->bIsNegative)
    {
        return -1 * (pBcd->ubyHundreds * 1000) + (pBcd->ubyTens * 100) + (pBcd->ubyUnits * 10) + (pBcd->ubyTenths);
    }
    else
    {
        return (pBcd->ubyHundreds * 1000) + (pBcd->ubyTens * 100) + (pBcd->ubyUnits * 10) + (pBcd->ubyTenths);
    }
}

void Alarm_Program_Update(void)
{    
    Timing_ReadTime();
    Eeprom_AlarmSettings* pSettings = &Eeprom_Settings;
    
    ushort usCurrentTimeInMins = CONVERT_HOURS_AND_MINS_TO_MINS(g_clock.hours, g_clock.minutes);
    if (usCurrentTimeInMins >= s_usDayProgramStartTimeInMins && usCurrentTimeInMins < s_usNightProgramStartTimeInMins)
    {
        s_CurrentProgram = ALARM_PROGRAM_DAY;
    }
    else
    {
        s_CurrentProgram = ALARM_PROGRAM_NIGHT;
    }
    
    Thermometer_ScratchPad sp;
    Thermometer_ReadScratchPad(&sp, 2);
    Thermometer_BcdTemperature tempBcd;
    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
    int sCurrentTemp = alarm_program_get_temp_as_short(&tempBcd);
    int sHighTemp = alarm_program_get_temp_as_short(pSettings);
    int sLowTemp = alarm_program_get_temp_as_short(((uchar*)pSettings) + 5);
    
    if (sCurrentTemp > sHighTemp || sCurrentTemp < sLowTemp)
    {
        Lcd_SetCursorPosition(6, 3);
        Lcd_WriteString("ALARM");
        PORTE = 0b001;
    }
    else
    {
        Lcd_SetCursorPosition(6, 3);
        Lcd_WriteString("     ");
        PORTE = 0b000;
    }
}

Alarm_Program_Programs Alarm_Program_GetProgram(void)
{
    return s_CurrentProgram;
}