/*
 * File:   timing.c
 * Author: billy
 *
 * Created on 04 November 2019, 13:40
 */
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
typedef unsigned char uchar;
typedef uchar bool;


void ReadSeconds();
void ReadMinutes();
void ReadHours();
void ReadDate();
void ReadMonth();
void ReadDay();
void ReadYear();
void CompleteOperation();
void WriteCommandByte(char);
char AssembleByte();

typedef struct
{
    char seconds;
    char minutes;
    char hours;
    char date;
    char day;
    char month;
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


inline void WriteCommandByte(char byte)
{    
    RST = 1; // Drive RST High
    
    //write the 8 bits of the command byte.
    for(int i = 0; i < 8; i++)
    {
        IO = (byte << i);
    }
}

inline char AssembleByte()
{
    char ret = 0;
    for(int i = 8; i > 0; i--)
    {
        ret |= (IO << i);
    }    
}

inline void memset(char* const ptr, const unsigned char c, const unsigned int len)
{
    for (char* p = ptr; p < p + len; p++)
        *p = c;
}
#define ZERO_MEMORY(ptr, type) memset(ptr, 0, sizeof(type))

void main(void) {
    TRISC =  0x00;
    PORTC = 0x55; 
    for (;;);
}

void Init()
{
    ZERO_MEMORY(&g_clock, Clock);      
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
    char temp = AssembleByte();
    char tens = (temp & 0x70);
    char digits = (temp & 0x07);    
    g_clock.seconds = (tens * 10) + digits;
}

void ReadMinutes()
{    
    char temp = AssembleByte();    
    char tens = (temp & 0x70);
    char digits = (temp & 0x07);    
    g_clock.minutes = (tens * 10) + digits;
}

void ReadHours()
{
    //TODO: double check assumptions made here based of bit defitions from the datasheet.   
    char temp = AssembleByte();    
    char digits = (temp & 0x0F);
    //assume 1 means 24 hr clock i.e. after 12am.
    char tens = 0;
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
    char temp = AssembleByte();
    char tens = (temp & 0x30);
    char digits = (temp & 0x0F);
    g_clock.date = (tens * 10) + digits;    
}

void ReadMonth()
{    
    char temp = AssembleByte();  
    char digits = (temp & 0x0F);
    char tens = (temp & 0x10);
    g_clock.month = (tens * 10) + digits;
}

void ReadDay()
{    
    char temp = AssembleByte();
    g_clock.day = (temp & 0x07);
}

void ReadYear()
{
    char temp = AssembleByte(); 
    char tens = (temp & 0xF0);
    char digits = (temp & 0x0F);
    g_clock.year = (tens * 10) + digits;
}


