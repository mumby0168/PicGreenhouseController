#include <xc.h>
#include "eeprom.h"

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
    for (uchar* p = &Eeprom_Settings; p < &Eeprom_Settings + sizeof(Eeprom_EepromSettings); p++)
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
    for (uchar* p = &Eeprom_Settings; p < &Eeprom_Settings + sizeof(Eeprom_EepromSettings); p++)
    {
        EEDATA = 0x00;
        
        EECON1bits.RD = EEPREOM_START_READ;
        *p = EEDATA;
        
        EEADR++;
    }
}