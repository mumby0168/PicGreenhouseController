```c
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

typedef struct _eeprom
{
    Eeprom_AlarmSettings daytimeAlarmSettings;
    Eeprom_AlarmSettings nighttimeAlarmSettings;
    const uchar ubyHasBeenBlanked;
} Eeprom_Eeprom;

bool Eeprom_Save(void);

void Eeprom_Load(void);

void Eeprom_LoadDefaultSettings(void);

void Eeprom_GetEeprom(Eeprom_Eeprom* const pEeprom);

void Eeprom_SetEeprom(const Eeprom_Eeprom* const pEeprom);

```