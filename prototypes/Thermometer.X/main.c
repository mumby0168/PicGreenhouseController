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

void WriteByte(uchar byte)
{
    for (char i = 0; i < 8; i++)
    {                 
    TRISA0 = 0;
    RA0 = 0;//shift the lowest bit                                                                             
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                     //pull high to low,produce write time    
   if ((byte >> i) & 1)  TRISA0 = 1;                    //if write 1,pull high                   
   delay(2,7);                                //delay 63us                             
   TRISA0 = 1;                                                                          
   NOP();                                                                              
   NOP();                                                                                                              //right shift a bit
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
        delay(2,7);                              //63us 
    }
       
    return ret;
}

uchar Reset(void)
{
    RA0 = 0;
    TRISA0 = 0;          

    delay(2,70);

    TRISA0 = 1;  

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

    delay(2,60);
    
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

uchar ReadScratchpad(ScratchPad* pScratchPad)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;

    WriteByte(0xBE);
    
    /*
    for (uchar* pSpEntry = pScratchPad; pSpEntry < pSpEntry + sizeof(ScratchPad); pSpEntry++)
        *pSpEntry = ReadByte();
    */
    
    g_ScratchPad.byTempLsb = ReadByte();
    g_ScratchPad.byTempMsb = ReadByte();
    
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
    
    WriteString("Start.");
    
    while (true)
    {   
        ClearDisplay();
        Reset();
        WriteByte(0x44);
        DelaySeconds(1);
        Reset();
        WriteByte(0xBE);
        char byOne = ReadByte();
        char byTwo = ReadByte();
        if (0 == 0)
        {
            WriteString("Success: ");
            SetCursorPosition(true, 0);
            PrintValue_Char(byOne);
            WriteCharacter(' ');
            PrintValue_Char(byTwo);
        }
        else
        {
            WriteString("Failed.");
        }
    };
    
    return;
}
