#include <xc.h>
#include "alarm_program.h"
#include "drivers/timing-chip.h"
#include "drivers/thermometer.h"
#include "drivers/eeprom.h"
#include "drivers/lcd.h"

typedef struct _alarm_time
{
    uchar minutes;
    uchar hours;
} AlarmTime;

static Alarm_Program_Programs s_CurrentProgram = ALARM_PROGRAM_DAY;
static AlarmTime s_DayProgramStartTime;
static AlarmTime s_NightProgramStartTime;

void Alarm_Program_Init(void)
{
    TRISE = 0b000;
    PORTE = 0b000;
    
    s_DayProgramStartTime.minutes = 30;
    s_DayProgramStartTime.hours = 6;
    
    s_NightProgramStartTime.minutes = 30;
    s_NightProgramStartTime.hours = 18;
}

void Alarm_Program_Update(void)
{    
    Timing_ReadTime();
    AlarmTime* pCurrentTime = &g_clock;
    Eeprom_AlarmSettings* pSettings = &Eeprom_Settings;
    
    if ((pCurrentTime->hours >= s_DayProgramStartTime.hours && pCurrentTime->hours <= s_DayProgramStartTime.minutes) &&
        (pCurrentTime->minutes >= s_DayProgramStartTime.minutes && pCurrentTime->minutes < s_DayProgramStartTime.minutes))
    {
        s_CurrentProgram = ALARM_PROGRAM_DAY;
    }
    else
    {
        s_CurrentProgram = ALARM_PROGRAM_NIGHT;
        pSettings += 1;
    }
    
    Thermometer_ScratchPad sp;
    Thermometer_ReadScratchPad(&sp, 2);
    Thermometer_BcdTemperature tempBcd;
    Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &tempBcd);
}

Alarm_Program_Programs Alarm_Program_GetProgram(void)
{
    return s_CurrentProgram;
}