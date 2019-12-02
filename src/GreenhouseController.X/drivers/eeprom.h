#ifndef EEPROM_H
#define EEPROM_H

#include "../libs/std.h"

typedef struct eepromSettings
{
    uchar temp;
} EepromSettings;

EepromSettings g_Settings;

static void eeprom_init_write();
bool Eeprom_Save();
void Eeprom_Load();

#endif