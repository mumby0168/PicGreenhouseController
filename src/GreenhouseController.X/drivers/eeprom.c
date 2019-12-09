#include <xc.h>
#include "eeprom.h"
#include "lcd.h"

#define EEPROM_DATA 0
#define EEPROM_DISABLE_WRITE 0
#define EEPROM_ENABLE_WRITE 1
#define EEPREOM_START_READ 1
#define EEPREOM_START_WRITE 1

bool Eeprom_Save()
{
    EECON1bits.EEPGD = EEPROM_DATA;
    EECON1bits.WREN = EEPROM_ENABLE_WRITE;
    //initialise the write
    
    EEADR = 0;
    EECON1bits.WRERR = 0;    
    uchar* pEepromSettings = &Eeprom_Settings;
    for (uchar* p = &Eeprom_Settings; p < pEepromSettings + sizeof(Eeprom_EepromSettings); p++)
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

void Eeprom_Load()
{
    //initialise the write
    EECON1bits.EEPGD = EEPROM_DATA;
    
    EEADR = 0;
    uchar* pEepromSettings = &Eeprom_Settings;
    for (uchar* p = &Eeprom_Settings; p < pEepromSettings + sizeof(Eeprom_EepromSettings); p++)
    {
        EEDATA = 0x00;
        
        EECON1bits.RD = EEPREOM_START_READ;
        *p = EEDATA;

        EEADR++;
    }
    
    if (Eeprom_Settings.ubyHasBeenBlanked == 0xFF)
    {
        for (uchar* p = &Eeprom_Settings; p < pEepromSettings + sizeof(Eeprom_EepromSettings); p++)
            *p = 0;
        
        //Load default settings
        //cold settings
        Eeprom_Settings.daytimeAlarmSettings.ubyColdAlarmTens = 1;
        Eeprom_Settings.daytimeAlarmSettings.ubyColdAlarmUnits = 8;
        Eeprom_Settings.nighttimeAlarmSettings.ubyColdAlarmTens = 1;
        Eeprom_Settings.nighttimeAlarmSettings.ubyColdAlarmUnits = 4;
        
        //hot settings
        Eeprom_Settings.daytimeAlarmSettings.ubyWarmAlarmTens = 2;
        Eeprom_Settings.daytimeAlarmSettings.ubyWarmAlarmUnits = 4;
        Eeprom_Settings.nighttimeAlarmSettings.ubyWarmAlarmTens = 2;
        Eeprom_Settings.nighttimeAlarmSettings.ubyWarmAlarmUnits = 4;
        
        Eeprom_Save();
    }
}