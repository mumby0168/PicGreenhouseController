#ifndef EEPROM_H
#define EEPROM_H

#include "../libs/std.h"

#define EEPROM_WARM_COLD_ALARM_BYTES 5

typedef struct _alarmSettings
{
    uchar ubyWarmAlarmNegative;
    uchar ubyWarmAlarmHundreds;
    uchar ubyWarmAlarmTens;
    uchar ubyWarmAlarmUnits;
    uchar ubyWarmAlarmTenths;
    uchar ubyColdAlarmNegative;
    uchar ubyColdAlarmHundreds;
    uchar ubyColdAlarmTens;
    uchar ubyColdAlarmUnits;
    uchar ubyColdAlarmTenths;
} Eeprom_AlarmSettings;

typedef struct _eepromSettings
{
    Eeprom_AlarmSettings daytimeAlarmSettings;
    Eeprom_AlarmSettings nighttimeAlarmSettings;
    const uchar ubyHasBeenBlanked;
} Eeprom_EepromSettings;

Eeprom_EepromSettings Eeprom_Settings;

static void eeprom_init_write();
bool Eeprom_Save();
void Eeprom_Load();

#endif