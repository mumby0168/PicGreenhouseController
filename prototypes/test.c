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
    char byIntegerPart = (ubyTempMsb & 0b00000111) << 4 | GetHighNibble(&ubyTempLsb);
    uchar ubySignedPart = ubyTempMsb & 0xF8;
    short sDecimalValue = 0;
    
    uchar byDecimalPart = GetLowNibble(&ubyTempLsb);
    
    //2^-4
    if (byDecimalPart & 0x01)
        sDecimalValue += 625;
    
    //2^-3
    if (byDecimalPart & 0x02)
        sDecimalValue += 1250;
    
    //2^-2
    if (byDecimalPart & 0x04)
        sDecimalValue += 2500;
    
    //2^-1
    if (byDecimalPart & 0x08)
        sDecimalValue += 5000;
    
    if (ubySignedPart != 0)
    {
        pBcdTemperature->bIsNegative = true;
        byIntegerPart -= 128;
        byIntegerPart *= -1;

        if (byDecimalPart != 0)
        {
            sDecimalValue -= 10000;
            sDecimalValue *= -1;
            byIntegerPart--;
        }
    }

    pBcdTemperature->ubyHundreds = (byIntegerPart / 100);
    pBcdTemperature->ubyTens = ((byIntegerPart / 10) % 10);
    pBcdTemperature->ubyUnits = (byIntegerPart % 10);
    pBcdTemperature->ubyTenths = ((sDecimalValue / 1000));
    pBcdTemperature->ubyHundredths = (((sDecimalValue % 1000) / 100));
    pBcdTemperature->ubyThousandths = (((sDecimalValue % 100) / 10));
    pBcdTemperature->ubyTenThousandths = ((sDecimalValue % 10));
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
    //-0.5
    Thermometer_BcdTemperature temperatureBcdValue;
    float temp = Thermometer_ConvertTempratureToFloat(0b11111111, 0b11111000);
    Thermometer_ConvertTempratureToBcd(0b11111111, 0b11111000, &temperatureBcdValue);

    printf("%f\n", temp);

    if (temperatureBcdValue.bIsNegative)
        printf("%c", '-');
        
    printf("%d", temperatureBcdValue.ubyHundreds);
    printf("%d", temperatureBcdValue.ubyTens);
    printf("%d", temperatureBcdValue.ubyUnits);
    printf("%c", '.');
    printf("%d", temperatureBcdValue.ubyTenths);
    printf("%d", temperatureBcdValue.ubyHundredths);
    printf("%d", temperatureBcdValue.ubyThousandths);
    printf("%d\n", temperatureBcdValue.ubyTenThousandths);

    //-10.125
    temp = Thermometer_ConvertTempratureToFloat(0b11111111, 0b01011110);
    Thermometer_ConvertTempratureToBcd(0b11111111, 0b01011110, &temperatureBcdValue);

    printf("%f\n", temp);

    if (temperatureBcdValue.bIsNegative)
        printf("%c", '-');
        
    printf("%d", temperatureBcdValue.ubyHundreds);
    printf("%d", temperatureBcdValue.ubyTens);
    printf("%d", temperatureBcdValue.ubyUnits);
    printf("%c", '.');
    printf("%d", temperatureBcdValue.ubyTenths);
    printf("%d", temperatureBcdValue.ubyHundredths);
    printf("%d", temperatureBcdValue.ubyThousandths);
    printf("%d\n", temperatureBcdValue.ubyTenThousandths);

    //-25.0625
    temp = Thermometer_ConvertTempratureToFloat(0b11111110, 0b01101111);
    Thermometer_ConvertTempratureToBcd(0b11111110, 0b01101111, &temperatureBcdValue);

    printf("%f\n", temp);

    if (temperatureBcdValue.bIsNegative)
        printf("%c", '-');
        
    printf("%d", temperatureBcdValue.ubyHundreds);
    printf("%d", temperatureBcdValue.ubyTens);
    printf("%d", temperatureBcdValue.ubyUnits);
    printf("%c", '.');
    printf("%d", temperatureBcdValue.ubyTenths);
    printf("%d", temperatureBcdValue.ubyHundredths);
    printf("%d", temperatureBcdValue.ubyThousandths);
    printf("%d\n", temperatureBcdValue.ubyTenThousandths);

    //-55
    temp = Thermometer_ConvertTempratureToFloat(0b11111100, 0b10010000);
    Thermometer_ConvertTempratureToBcd(0b11111100, 0b10010000, &temperatureBcdValue);

    printf("%f\n", temp);

    if (temperatureBcdValue.bIsNegative)
        printf("%c", '-');
        
    printf("%d", temperatureBcdValue.ubyHundreds);
    printf("%d", temperatureBcdValue.ubyTens);
    printf("%d", temperatureBcdValue.ubyUnits);
    printf("%c", '.');
    printf("%d", temperatureBcdValue.ubyTenths);
    printf("%d", temperatureBcdValue.ubyHundredths);
    printf("%d", temperatureBcdValue.ubyThousandths);
    printf("%d\n", temperatureBcdValue.ubyTenThousandths);
}