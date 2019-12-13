#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "../libs/std.h"

typedef struct _thermometer_userConfig
{
    uchar ubyUserOne;
    uchar ubyUserTwo;
    uchar ubyConfig;   
} Thermometer_UserConfig;

typedef struct _thermometer_scratchPad
{
    uchar ubyTempLsb;
    uchar ubyTempMsb;
    Thermometer_UserConfig userConfig;
    uchar ubyRes1;
    uchar ubyRes2;
    uchar ubyRes3;
    uchar ubyCRC;
} Thermometer_ScratchPad;

typedef struct _thermometer_bcdTemperature
{
    bool bIsNegative;
    uchar ubyHundreds;
    uchar ubyTens;
    uchar ubyUnits;
    uchar ubyTenths;
    uchar ubyHundredths;
    uchar ubyThousandths;
    uchar ubyTenThousandths;
} Thermometer_BcdTemperature;

volatile bool Thermometer_bProcessTemperatureComplete;

void Thermometer_Init(void);

void Thermometer_Update(void);

uchar Thermometer_ProcessTemperature(void);

uchar Themometer_WriteScratchPad(const Thermometer_UserConfig userConfig);

uchar Thermometer_ReadScratchPad(Thermometer_ScratchPad* pScratchPad, uchar byBytesToRead);

uchar Thermometer_CopyScratchPadToE2(void);

uchar Thermometer_CopyE2ToScratchPad(void);

float Thermometer_ConvertTempratureToFloat(uchar ubyTempMsb, uchar ubyTempLsb);

void Thermometer_ConvertTempratureToBcd(uchar ubyTempMsb, uchar ubyTempLsb, Thermometer_BcdTemperature* pBcdTemperature);

short Thermometer_ConvertTempratureToShort(const Thermometer_BcdTemperature* const pBcd);


#endif