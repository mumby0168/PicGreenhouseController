#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "../libs/std.h"

typedef struct _thermometer_userConfig
{
    uchar byUserOne;
    uchar byUserTwo;
    uchar byConfig;   
} Thermometer_UserConfig;

typedef struct _scratchPad
{
    uchar byTempLsb;
    uchar byTempMsb;
    Thermometer_UserConfig userConfig;
    uchar byRes1;
    uchar byRes2;
    uchar byRes3;
    uchar byCRC;
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

uchar Thermometer_ProcessTemperature(void);
uchar Themometer_WriteScratchPad(const Thermometer_UserConfig userConfig);
uchar Thermometer_ReadScratchPad(Thermometer_ScratchPad* pScratchPad, uchar byBytesToRead);
uchar Thermometer_CopyScratchPadToE2(void);
uchar Thermometer_CopyE2ToScratchPad(void);
float Thermometer_ConvertTempratureToFloat(uchar ubyTempMsb, uchar ubyTempLsb);
void Thermometer_ConvertTempratureToBcd(uchar ubyTempMsb, uchar ubyTempLsb, Thermometer_BcdTemperature* pBcdTemperature);

#endif