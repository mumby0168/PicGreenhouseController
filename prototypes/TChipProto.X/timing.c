/*
 * File:   timing.c
 * Author: billy
 *
 * Created on 04 November 2019, 13:40
 */

#include "lcd.h"
#include <xc.h> 

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

/* to timing .h file start*/

#define RST RB5
#define IO RB4
#define SCLK RB0

//begin command defs
#define WRITE_BURST 0xBE
#define READ_BURST 0xBF

#define WRITE_SECONDS 0x80
#define READ_SECONDS 0x81

#define WRITE_MINUTES 0x82
#define READ_MINUTES 0x83

#define WRITE_HOURS 0x84
#define READ_HOURS 0x85

#define WRITE_DATE 0x86
#define READ_DATE 0x87

#define WRITE_MONTH 0x88
#define READ_MONTH 0x89

#define WRITE_DAY 0x8A
#define READ_DAY 0x8B

#define WRITE_YEAR 0x8C
#define READ_YEAR 0x8D

// end command defs

typedef unsigned short int ushort;


//Write Operations
void WriteDate(uchar data);
void WriteSeconds(uchar seconds);
void WriteHours(uchar hours);
void WriteMinutes(uchar minutes);


//Read Operations
void ReadSeconds();
void ReadMinutes();
void ReadHours();
void ReadDate();
void ReadMonth();
void ReadDay();
void ReadYear();

//Standard Functions
void CompleteOperation();
void WriteCommandByte(uchar);
uchar AssembleByte();
void WriteByte(uchar*);

void Init();

typedef struct
{
    uchar seconds;
    uchar minutes;
    uchar hours;
    uchar date;
    uchar day;
    uchar month;
    ushort year;
} Clock;


Clock g_clock;

/* to timing .h file end*/


//This MUST be called prior to every operation makes sense to do this externally for now.
inline void CompleteOperation()
{
    RST = 0;
}

inline bool IsBitSet(uchar* byValue, uchar byBitOffset)
{
    return (*byValue >> byBitOffset) & 1U;
}

inline void SetBitHigh(uchar* byValue, uchar byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}


inline void WriteCommandByte(uchar byte)
{    
    TRISB0 = 0;
    SCLK = 0;
    RST = 1; // Drive RST High
    TRISB0 = 1;
    
    //write the 8 bits of the command byte.
    for(int i = 0; i < 8; i++)
    {
        IO = (byte >> i) | 1 ? 1 : 0;
    }
}

void WriteByte(uchar *byte)
{
    char counter = 0;
    while(counter < 8)
    {
        if(SCLK == 1)
        {
            IO = (*byte >> counter) | 1 ? 1 : 0;
            counter++;
        }

        while(SCLK == 1);
    }    
}

inline uchar AssembleByte()
{
    uchar ret = 0;

    char counter = 0;

    while (counter < 8)
    {
        if(SCLK == 0)
        {
            ret |= (IO << counter);
            counter++;
        }

        while (SCLK == 0);        
    }        
    return ret;
}

void Timing_PrepareWrite()
{
    TRISB4 = 0;
    TRISB5 = 0;        
}

void Timing_PrepareRead()
{
    TRISB4 = 1;
    TRISB5 = 0;    
}

inline void memset(void* const ptr, const unsigned char c, const unsigned int len)
{
    for (char* p = (char*)ptr; p < p + len; p++)
        *p = c;
}

#define ZERO_MEMORY(ptr, type) memset(ptr, 0, sizeof(type))

void main(void) {
    
    Init();
    Initialise();
    
    ClearDisplay();
    SetDisplayResolution(true, false);    
    
    Timing_PrepareWrite();
    WriteCommandByte(WRITE_SECONDS);    
    WriteHours(0);
    CompleteOperation();    
    

    while (true)
    {
        Timing_PrepareWrite();
        WriteCommandByte(READ_SECONDS);        
        Timing_PrepareRead();
        ReadSeconds();
        CompleteOperation();        
        WriteNumber(g_clock.hours);        
        ClearDisplay();
        SetDisplayMode(true, false, false);      
    }
    

}

void Init()
{
    TRISB = 1;
    ZERO_MEMORY(&g_clock, Clock);      
}

void WriteSeconds(uchar seconds)
{
    uchar encoded = 0;
    uchar tens = seconds / 10;
    uchar digits = seconds % 10;
    encoded = digits;
    encoded |= (tens << 4);
    WriteByte(&encoded);
}

void WriteMinutes(uchar minutes)
{
    uchar encoded = 0;
    uchar tens = minutes / 10;
    uchar digits = minutes % 10;
    encoded = digits;
    encoded |= (tens << 4);
    WriteByte(&encoded);
}

void WriteHours(uchar hours)
{
    uchar encoded = 0;
    if(hours > 12) SetBitHigh(&encoded, 7);
    uchar tens = hours / 10;
    uchar digits = hours % 10;
    encoded = digits;
    encoded |= (tens << 3);
    WriteByte(&encoded);
}


void BurstRead()
{
    WriteCommandByte(READ_BURST);
    ReadSeconds();
    ReadMinutes();
    ReadHours();
    ReadDate();
    ReadMonth();
    ReadDay();
    ReadYear();
    CompleteOperation();
}

void ReadSeconds()
{    
    uchar temp = AssembleByte();
    uchar tens = (temp & 0x70);
    uchar digits = (temp & 0x07);    
    g_clock.seconds = (tens * 10) + digits;
}

void ReadMinutes()
{    
    uchar temp = AssembleByte();    
    uchar tens = (temp & 0x70);
    uchar digits = (temp & 0x07);    
    g_clock.minutes = (tens * 10) + digits;
}

void ReadHours()
{
    //TODO: double check assumptions made here based of bit defitions from the datasheet.   
    uchar temp = AssembleByte();    
    uchar digits = (temp & 0x0F);
    //assume 1 means 24 hr clock i.e. after 12am.
    uchar tens = 0;
    if(IsBitSet(&temp, 7))
    {
        tens = (temp & 0x30);
    }
    else
    {
        tens = IsBitSet(&temp, 4) ? 1 : 0;
    }        
    g_clock.minutes = (tens * 10) + digits;
}

void ReadDate()
{
    uchar temp = AssembleByte();
    uchar tens = (temp & 0x30);
    uchar digits = (temp & 0x0F);
    g_clock.date = (tens * 10) + digits;    
}

void ReadMonth()
{    
    uchar temp = AssembleByte();  
    uchar digits = (temp & 0x0F);
    uchar tens = (temp & 0x10);
    g_clock.month = (tens * 10) + digits;
}

void ReadDay()
{    
    uchar temp = AssembleByte();
    g_clock.day = (temp & 0x07);
}

void ReadYear()
{
    uchar temp = AssembleByte(); 
    uchar tens = (temp & 0xF0);
    uchar digits = (temp & 0x0F);
    g_clock.year = (tens * 10) + digits;
}


