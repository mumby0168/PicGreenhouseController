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
static bool s_bIsHeating = false;
static bool s_bIsCooling = false;
static short s_sPrevTemp = 0;

bool Alarm_Program_IsHeating(void)
{
    return s_bIsHeating;
}

bool Alarm_Program_IsCooling(void)
{
    return s_bIsCooling;
}

void Alarm_Program_Init(void)
{
    TRISE = 0b000;
    PORTE = 0b000;
    TRISB6 = 0;
    TRISB7 = 0;
    RB6 = 0;
    RB7 = 0;
    
    Alarm_Program_Update();
}

void Alarm_Program_Update(void)
{    
    Timing_ReadTime();
    Eeprom_Eeprom eepromSettings;
    Eeprom_GetEeprom(&eepromSettings);
    Eeprom_AlarmSettings* pSettings = &eepromSettings;
    
    Clock clock;
    Timing_ReadClock(&clock);
    
    ushort usCurrentTimeInMins = CONVERT_HOURS_AND_MINS_TO_MINS(clock.ucHours, clock.ucMinutes);
    if (usCurrentTimeInMins >= s_usDayProgramStartTimeInMins && usCurrentTimeInMins < s_usNightProgramStartTimeInMins)
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
    int sCurrentTemp = Thermometer_ConvertTempratureToShort(&tempBcd);
    int sHighTemp = Thermometer_ConvertTempratureToShort(pSettings);
    int sLowTemp = Thermometer_ConvertTempratureToShort(((uchar*)pSettings) + 5);
    
    if (sCurrentTemp > sHighTemp)
    {
        s_bIsCooling = true;
        s_bIsHeating = false;
        RB6 = 0;
        RB7 = 1;
        
        if (s_sPrevTemp - sCurrentTemp < 0) //if the vector from current to prev is negative or the same then it is not cooling so sound alarm
        {
            PORTE = 0b001;
        }
        else
        {
            PORTE = 0b000;
        }
    }
    else if (sCurrentTemp < sLowTemp)
    {
        s_bIsHeating = true;
        s_bIsCooling = false;
        RB6 = 1;
        RB7 = 0;
        
        if (s_sPrevTemp - sCurrentTemp > 0) //if the vector from current to prev is positive or the same then it is not warming so sound alarm
        {
            PORTE = 0b001;
        }
        else
        {
            PORTE = 0b000;
        }
    }
    else
    {
        s_bIsHeating = false;
        s_bIsCooling = false;
        RB6 = 0;
        RB7 = 0;
        PORTE = 0b000;
    }
    
    s_sPrevTemp = sCurrentTemp;
}

Alarm_Program_Programs Alarm_Program_GetProgram(void)
{
    return s_CurrentProgram;
}