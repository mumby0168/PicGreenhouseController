#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef unsigned char uchar;
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

uchar GetLowNibble(uchar* val)
{
    return *val & 0x0F;
}

uchar GetHighNibble(uchar* val)
{
    return (*val & 0xF0) >> 4;
}

void Thermometer_ConvertTempratureToBcd(uchar ubyTempMsb, uchar ubyTempLsb, Thermometer_BcdTemperature* pBcdTemperature)
{
    pBcdTemperature->bIsNegative = false;
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    uchar ubyIntegerPart = (ubyTempMsb & 0x07) << 4 | GetHighNibble(&ubyTempLsb);
    uchar ubySignedPart = ubyTempMsb & 0xF8;
    
    if (ubySignedPart != 0)
        pBcdTemperature->bIsNegative = true;
        
    pBcdTemperature->ubyHundreds = (ubyIntegerPart / 100) + 48;
    pBcdTemperature->ubyTens = ((ubyIntegerPart / 10) % 10) + 48;
    pBcdTemperature->ubyUnits = (ubyIntegerPart % 10) + 48;

    uchar byDecimalPart = GetLowNibble(&ubyTempLsb);
    
    unsigned short usDecimalValue = 0;
    //2^-4
    if (byDecimalPart & 0x01)
        usDecimalValue += 625;
    
    //2^-3
    if (byDecimalPart & 0x02)
        usDecimalValue += 1250;
    
    //2^-2
    if (byDecimalPart & 0x04)
        usDecimalValue += 2500;
    
    //2^-1
    if (byDecimalPart & 0x08)
        usDecimalValue += 5000;
    
    pBcdTemperature->ubyTenths = ((usDecimalValue / 1000) + 48);
    pBcdTemperature->ubyHundredths = (((usDecimalValue % 1000) / 100) + 48);
    pBcdTemperature->ubyThousandths = (((usDecimalValue % 100) / 10) + 48);
    pBcdTemperature->ubyTenThousandths = ((usDecimalValue % 10) + 48);
}

float Thermometer_ConvertTempratureToFloat(uchar ubyTempMsb, uchar ubyTempLsb)
{
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    uchar byIntegerPart = (ubyTempMsb & 0x07) << 4 | GetHighNibble(&ubyTempLsb);
    //Get the low nibble, least significant bit is 2^-4 so divide by 2^4 = 16
    float fDecimalPart = (float)GetLowNibble(&ubyTempLsb) / 16;
    uchar ubySignedPart = ubyTempMsb & 0xF8;
    if (ubySignedPart != 0)
        return (float)byIntegerPart + fDecimalPart - 128;
    
    return (float)byIntegerPart + fDecimalPart;
}

int main()
{
    Thermometer_BcdTemperature temperatureBcdValue;
    float temp = Thermometer_ConvertTempratureToFloat(0b11111111, 0b11111000);       
    printf("%f", temp);
}