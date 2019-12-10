#include <xc.h>
#include "eeprom.h"
#include "lcd.h"

#define EEPROM_DATA 0
#define EEPROM_DISABLE_WRITE 0
#define EEPROM_ENABLE_WRITE 1
#define EEPREOM_START_READ 1
#define EEPREOM_START_WRITE 1

static Eeprom_Eeprom s_Eeprom;

bool Eeprom_Save(void)
{
    EECON1bits.EEPGD = EEPROM_DATA;
    EECON1bits.WREN = EEPROM_ENABLE_WRITE;
    //initialise the write
    
    EEADR = 0;
    EECON1bits.WRERR = 0;    
    uchar* pEepromSettings = &s_Eeprom;
    for (uchar* p = &s_Eeprom; p < pEepromSettings + sizeof(Eeprom_Eeprom); p++)
    {
        PIR2bits.EEIF = 0; //clear the eeprom finished interrupt flag.
        
        //wait for previous write to finish
        while (EECON1bits.WR);
        
        EEDATA = *p;
        
        //start write sequence
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = EEPREOM_START_WRITE;
        
        while (PIR2bits.EEIF == 0);
        
        if (EECON1bits.WRERR == 1)
            return false;
        
        EEADR++;
    }
    
    EECON1bits.WREN = EEPROM_DISABLE_WRITE;
    PIR2bits.EEIF = 0;
        
    return true;
}

void Eeprom_Load(void)
{
    //initialise the write
    EECON1bits.EEPGD = EEPROM_DATA;
    
    EEADR = 0;
    uchar* pEeprom = &s_Eeprom;
    for (uchar* p = &s_Eeprom; p < pEeprom + sizeof(Eeprom_Eeprom); p++)
    {
        EEDATA = 0x00;
        
        EECON1bits.RD = EEPREOM_START_READ;
        *p = EEDATA;

        EEADR++;
    }
    
    if (s_Eeprom.ubyHasBeenBlanked == 0xFF)
    {
        for (uchar* p = &s_Eeprom; p < pEeprom + sizeof(Eeprom_Eeprom); p++)
            *p = 0;
        
        Eeprom_LoadDefaultSettings();
        
        Eeprom_Save();
    }
}

void Eeprom_LoadDefaultSettings(void)
{
    //Load default settings
    //cold settings
    s_Eeprom.daytimeAlarmSettings.ubyColdAlarmTens = 1;
    s_Eeprom.daytimeAlarmSettings.ubyColdAlarmUnits = 8;
    s_Eeprom.nighttimeAlarmSettings.ubyColdAlarmTens = 1;
    s_Eeprom.nighttimeAlarmSettings.ubyColdAlarmUnits = 4;

    //hot settings
    s_Eeprom.daytimeAlarmSettings.ubyWarmAlarmTens = 2;
    s_Eeprom.daytimeAlarmSettings.ubyWarmAlarmUnits = 4;
    s_Eeprom.nighttimeAlarmSettings.ubyWarmAlarmTens = 2;
    s_Eeprom.nighttimeAlarmSettings.ubyWarmAlarmUnits = 4;
}

void Eeprom_GetEeprom(Eeprom_Eeprom* const pEepromCopy)
{
    uchar* pEeprom = &s_Eeprom;
    for (uchar i = 0; i < sizeof(Eeprom_Eeprom); i++)
        *((uchar*)pEepromCopy + i) = *(pEeprom + i);
}

void Eeprom_SetEeprom(const Eeprom_Eeprom* const pEepromCopy)
{
    uchar* pEeprom = &s_Eeprom;
    for (uchar i = 0; i < sizeof(Eeprom_Eeprom); i++)
        *(pEeprom + i) = *((uchar*)pEepromCopy + i);
}