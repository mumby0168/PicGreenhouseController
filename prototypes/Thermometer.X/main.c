#include <xc.h>
#include "std.h"
#include "BitUtillities.h"
#include "Delays.h"
#include "lcd.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

void delay(char x,char y) 
{
  char z;
  do{
      z=y;
      do{;}while(--z);
     }while(--x);
}

#include "example.c"

void WriteByte(uchar val)
{
    uchar byBitValue;
    for(uchar i = 0; i < 8; i++)
    {
        byBitValue = (val >> i) & 1;                             //shift the lowest bit                   
        
        RA0 = 0;
        TRISA0 = 0;
        
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                     //pull high to low,produce write time    
        
        if(byBitValue == 1)
            TRISA0 = 1; //if write 1,pull high   
        
        delay(2, 7); //63us
        TRISA0 = 1;                                                                          
        
        NOP();                                                                              
        NOP();                                     //right shift a bit                      
     }
    
    TRISA0 = 1;
}

uchar ReadByte(void)
{ 
    uchar ret = 0;    
    for (char i = 0; i < 8; i++)
    {
        RA0 = 0;
        TRISA0 = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();                                    //6us              
        TRISA0 = 1;      
        NOP();                                                       
        NOP();                                                       
        NOP();                                                       
        NOP();                                                       
        NOP();                                   //4us               
        ret |= RA0 << i;                                        
        DelayMicroSeconds(63);                             //63us 
    }
       
    return ret;
}

uchar Reset(void)
{
    RA0 = 0;
    TRISA0 = 0;          

    DelayMicroSeconds(504);

    TRISA0 = 1;  

    uchar uiElapsed = 0;
    while (RA0 == 1)
    {
        if (uiElapsed > 120)
            return 0x01;

        uiElapsed += 2;
    }             

    uiElapsed = 0;
    while (RA0 == 0)
    {
        if (uiElapsed > 240)
            return 0x02;

        uiElapsed += 2;
    };

    DelayMicroSeconds(430);
    
    WriteByte(0xCC); //skip rom
    
    return 0;
}

uchar ConvertT(void)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0x44);
    return 0;
}

typedef struct _userConfig
{
    uchar byUserOne;
    uchar byUserTwo;
    uchar byConfig;   
} UserConfig;

typedef struct _scratchPad
{
    uchar byTempLsb;
    uchar byTempMsb;
    UserConfig userConfig;
    uchar byRes1;
    uchar byRes2;
    uchar byRes3;
    uchar byCRC;
} ScratchPad;

uchar WriteScratchPad(const UserConfig userConfig)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0x4E);
    
    for (uchar* pUcEntry = &userConfig; pUcEntry < pUcEntry + sizeof(UserConfig); pUcEntry++)
        WriteByte(*pUcEntry);
    
    return byStatus;
}

uchar ReadScratchPad(ScratchPad* pScratchPad, uchar byBytesToRead)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;

    WriteByte(0xBE);
    
    if (byBytesToRead > sizeof(ScratchPad))
        byBytesToRead = sizeof(ScratchPad);
    
    for (uchar* pSpEntry = pScratchPad; pSpEntry < pSpEntry + byBytesToRead; pSpEntry++)
        *pSpEntry = ReadByte();
    
    byStatus = Reset();
    
    return byStatus;
}

uchar CopyScratchPadToE2()
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0x48);
    
    return byStatus;
}

uchar CopyE2ToScratchPad()
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0xB8);
    
    return byStatus;
}

inline char GetHundredsDigit(char n)
{
    return (n / 100) + 48;
}

inline char GetTensDigit(char n)
{
    return ((n / 10) % 10) + 48;
}

inline char GetUnitsDigit(char n)
{
    return (n % 10) + 48;
}

void PrintValue_Char(char c)
{
    WriteCharacter(GetHundredsDigit(c));
    WriteCharacter(GetTensDigit(c));
    WriteCharacter(GetUnitsDigit(c));
}

float ConvertTempToFloat(uchar lsb, uchar msb)
{
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    uchar byIntegerPart = (msb & 0x07) << 4 | GetHighNibble(&lsb);
    //Get the low nibble, least significant bit is 2^-4 so divide by 2^4 = 16
    float fDecimalPart = (float)GetLowNibble(lsb) / 16;
    
    return (float)byIntegerPart + fDecimalPart;
}

void PrintTemp(uchar lsb, uchar msb)
{
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    uchar byIntegerPart = (msb & 0x07) << 4 | GetHighNibble(&lsb);
    uchar byDecimalPart = GetLowNibble(&lsb);
    PrintValue_Char(byIntegerPart);
    WriteCharacter('.');
    
    unsigned short usDecimalValue = 0;
    uchar res = byDecimalPart & 0x01;
    //2^-4
    if (res == 0x01)
    {
        usDecimalValue += 625;
    }
    
    //2^-3
    res = byDecimalPart & 0x02;
    if (res == 0x02)
    {
        usDecimalValue += 1250;
    }
    
    //2^-2
    res = byDecimalPart & 0x04;
    if (res == 0x04)
    {
        usDecimalValue += 2500;
    }
    
    //2^-1
    res = byDecimalPart & 0x08;
    if (res == 0x08)
    {
        usDecimalValue += 5000;
    }
    
    WriteCharacter((usDecimalValue / 1000) + 48);
    WriteCharacter(((usDecimalValue % 1000) / 100) + 48);
    WriteCharacter(((usDecimalValue % 100) / 10) + 48);
    WriteCharacter((usDecimalValue % 10) + 48);    
}

void main(void) 
{
    Initialise();
    ClearDisplay();
    SetDisplayResolution(true, false); 
    SetDisplayMode(true, false, false);
    
    WriteString("Start.");
    
    UserConfig userConfig;
    while (true)
    {          
        ClearDisplay();
        
        uchar byRes = ConvertT();
        
        DelayMilliSeconds(750);
        
        ScratchPad sp;
        byRes = ReadScratchPad(&sp, sizeof(ScratchPad));
        userConfig = sp.userConfig;
        
        if (byRes == 0)
        {
            WriteString("Success: ");
            SetCursorPosition(true, 0);
            PrintTemp(sp.byTempLsb, sp.byTempMsb);
        }
        else
        {
            WriteString("Failed: ");
            PrintValue_Char(byRes);
        }
        
        DelaySeconds(1);
    };
    
    return;
}
