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

void WriteZero(void)
{
    TRISA0 = 0;
    RA0 = 0;
    DelayMicroSeconds(75);
}

void WriteOne(void)
{
    DelayMicroSeconds(2);
    TRISA0 = 0;
    RA0 = 0;
    
    DelayMicroSeconds(10);
    RA0 = 1;
    DelayMicroSeconds(60);
}

void WriteByte(uchar byte)
{
    for (char i = 0; i < 8; i++)
    {
        if ((byte >> i) & 1)
        {
            WriteOne();
        }
        else
        {
            WriteZero();
        }
    }
    
    TRISA0 = 1;
}

char ReadBit(void)
{
    TRISA0 = 0;
    RA0 = 0;
    
    DelayMicroSeconds(8);
    
    TRISA0 = 1;
    
    char ret = RA0;    
    return ret;
}

uchar ReadByte(void)
{
    uchar ret = 0;    
    for (char i = 0; i < 8; i++)
        ret |= ReadBit() << i;
    
    return ret;
}

inline void SkipRom(void)
{
    WriteByte(0xCC);
}

uchar Reset(void)
{
    char presence=1;
    while(presence)
    { 
        ClearDisplay();
        WriteCharacter(48);
        RA0 = 0;
        TRISA0 = 0;//MAIN MCU PULL LOW                                                                       
        DelayMicroSeconds(503);                              //delay 503us                                                                             
        TRISA0 = 1;                                //release general line and wait for resistance pull high general line and keep 15~60us    
        uchar uiElapsed = 0;
        while (RA0 == 1)
        {
            if (uiElapsed > 120)
                return 0x12;

            uiElapsed += 2;
        }                                                                            
        uiElapsed = 0;
        while (RA0 == 0)
        {
            if (uiElapsed > 240)
                return 0x13;

            uiElapsed += 2;
        };
        presence = 0;//receive responsion signal                                                               
        DelayMicroSeconds(430);                           //delay 430us                                                                             
     }
    
    /*
    //Bring RA0 low for 500us
    RA0 = 0;
    TRISA0 = 0;
    
    DelayMicroSeconds(600);
    
    //Release control of RA0
    TRISA0 = 1;
    
    //wait for it to return to high
    unsigned int uiElapsed = 0;
    while (RA0 == 0)
    {
        if (uiElapsed > 60)
            return 0x11;
        
        uiElapsed += 2;
    }
        
    //Wait for the thermomiter to pull RA0 low
    uiElapsed = 0;
    while (RA0 == 1)
    {
        if (uiElapsed > 60)
            return 0x12;
        
        uiElapsed += 2;
    }
    
    uiElapsed = 0;
    while (RA0 == 0)
    {
        if (uiElapsed > 240)
            return 0x13;
        
        uiElapsed += 2;
    };
    
    DelayMicroSeconds(480);
    
    SkipRom();
    
    return 0;
     * */
}

uchar ConvertT(void)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0x44);
    return 0;
}

typedef struct _scratchPad
{
    uchar byTempLsb;
    uchar byTempMsb;
    uchar byUserOne;
    uchar byUserTwo;
    uchar byConfig;
    uchar byRes1;
    uchar byRes2;
    uchar byRes3;
    uchar byCRC;
} ScratchPad;

ScratchPad g_ScratchPad;

uchar ReadScratchpad()
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;

    WriteByte(0x4E);
    
//    for (int i = 0; i < 9; i++)
//    {
//        ReadByte();
//    }
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
//    g_ScratchPad.byTempLsb = ReadByte();
    
    byStatus = Reset();
    
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

void main(void) 
{
    Initialise();
    ClearDisplay();
    SetDisplayResolution(true, false); 
    SetDisplayMode(true, false, false);
    while (true)
    {        
        uchar byRes = Reset();
        ClearDisplay();
        if (byRes == 0)
        {
            WriteString("Success");
            WriteCharacter(58);
            PrintValue_Char(128);
        }
        else
        {
            WriteString("Failed.");
        }
        DelaySeconds(2);
    };
    
    return;
}
