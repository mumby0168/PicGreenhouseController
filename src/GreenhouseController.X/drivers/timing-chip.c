/***********************************************************************
 * NAME: timing-chip.c
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#include "timing-chip.h"
#include "../libs/BinaryUtillities.h"
#include <xc.h>


/**************************Defines*****************************/

#define RST RB5
#define IO RB4
#define SCLK RB0

#define SCKL_CONFIG TRISB0
#define IO_CONFIG TRISB4
#define RST_CONFIG TRISB5

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

#define CONTROL_CMD 0x8E

#define COMPLETE_OPERATION RST = 0;


/*********************Forward Declarations*********************/

inline void memset(void* const ptr, const unsigned char c, const unsigned int len)
{
    for (char* p = (char*)ptr; p < p + len; p++)
        *p = c;
}

#define ZERO_MEMORY(ptr, type) memset(ptr, 0, sizeof(type))

//Write Operations
static void WriteDate(uchar data);
static void WriteSeconds(uchar seconds);
static void WriteHours(uchar hours);
static void WriteMinutes(uchar minutes);
static void WriteDate(uchar date);
static void WriteMonth(uchar month);
static void WriteDay(uchar day);
static void WriteYear(uchar year);



//Read Operations
static void ReadSeconds(void);
static void ReadMinutes(void);
static void ReadHours(void);
static void ReadDate(void);
static void ReadMonth(void);
static void ReadDay(void);
static void ReadYear(void);

//Low Level API
inline void WriteCommandByte(uchar);
uchar AssembleByte();
void WriteByte(uchar*);

/***********************Implementation*************************/

// High Level API

void Timing_Init()
{
    //Init structs    
    ZERO_MEMORY(&g_clock, Clock);     
    ZERO_MEMORY(&g_rawClock, RawClock);    

    //set clock as output & send 0.
    SCKL_CONFIG = 0;
    SCLK = 0;
    RST_CONFIG = 0;

    //enables the clock.
    RST = 0;
    RST = 1;
    // IO as output
    IO_CONFIG = 0;
    // speak to sir to find out where docs are for these operations
    WriteCommandByte(CONTROL_CMD);
    WriteByte(0); 

    COMPLETE_OPERATION // Drives reset low.
}

void Timing_SetCalendar(uchar day, uchar date, uchar month, uchar year)
{
    WriteCommandByte(WRITE_DAY);
    WriteDay(day);
    COMPLETE_OPERATION

    WriteCommandByte(WRITE_DATE);
    WriteDate(date);
    COMPLETE_OPERATION

    WriteCommandByte(WRITE_MONTH);
    WriteMonth(month);
    COMPLETE_OPERATION

    WriteCommandByte(WRITE_YEAR);
    WriteYear(year);
    COMPLETE_OPERATION
}

void Timing_SetTime(uchar hours, uchar minutes, uchar seconds)
{
    WriteCommandByte(WRITE_HOURS);
    WriteHours(hours);
    COMPLETE_OPERATION

    WriteCommandByte(WRITE_MINUTES);
    WriteMinutes(minutes);
    COMPLETE_OPERATION

    WriteCommandByte(WRITE_SECONDS);
    WriteSeconds(seconds);
    COMPLETE_OPERATION
}

void Timing_ReadTime()
{
    WriteCommandByte(READ_HOURS);
    ReadHours();
    COMPLETE_OPERATION

    WriteCommandByte(READ_MINUTES);
    ReadMinutes();
    COMPLETE_OPERATION    

    WriteCommandByte(READ_SECONDS);
    ReadSeconds();
    COMPLETE_OPERATION
}


void Timing_ReadCalendar()
{
    WriteCommandByte(READ_DAY);
    ReadDay();
    COMPLETE_OPERATION

    WriteCommandByte(READ_DATE);
    ReadDate();
    COMPLETE_OPERATION

    WriteCommandByte(READ_MONTH);
    ReadMonth();
    COMPLETE_OPERATION

    WriteCommandByte(READ_YEAR);
    ReadYear();
    COMPLETE_OPERATION
}


//Low Level API

inline void WriteCommandByte(uchar byte)
{    
    //set io pin to output
    IO_CONFIG = 0;    
    RST = 1; // Drive RST High        
    //write the 8 bits of the command byte.
    WriteByte(&byte);
}

void WriteByte(uchar *byte)
{    
    IO_CONFIG = 0;
    char i;
     for (i = 0; i < 8; i++)
    {          
        SCLK = 0; // Ensure pull up before placing bit.
        IO = (*byte >> i) & 1;        
        SCLK = 1; // Ensure pull down after writing bit.        
    } 
    SCLK = 0;
}

inline uchar AssembleByte()
{
    IO_CONFIG = 1;
    uchar ret = 0;    
    uchar temp = 0;
    int i;
    for (i = 0; i < 8; i++)
    {        
        temp = 0;
        SCLK = 0; // drive low to read bit.        
        temp = IO & 1;        
        SCLK = 1; // drive back high
        ret |= (temp << i);          
    }           
    SCLK = 0;
    return(ret);
}

//Write Operations

static void WriteSeconds(uchar seconds)
{
    uchar encoded = 0;
    uchar tens = seconds / 10;
    uchar digits = seconds % 10;
    encoded = digits;
    encoded |= (tens << 4);
    WriteByte(&encoded);
}

static void WriteMinutes(uchar minutes)
{
    uchar encoded = 0;
    uchar tens = minutes / 10;
    uchar digits = minutes % 10;
    encoded = digits;
    encoded |= (tens << 4);
    WriteByte(&encoded);
}

static void WriteHours(uchar hours)
{
    uchar encoded = 0;
    uchar tens = hours / 10;    
    
    uchar digits = hours % 10;    
    encoded = digits;    
    if(hours > 12) 
    {
        SetBitHigh(&encoded, 7);        
    }
        
    if(hours > 9)
    {
        SetBitHigh(&encoded, 4);  
    }
    
    WriteByte(&encoded);
}

static void WriteDate(uchar date)
{
    uchar digits = date % 10;
    uchar tens = date / 10;    
    uchar encoded = digits;
    encoded |= (tens << 4);
    WriteByte(&encoded);    
}

static void WriteMonth(uchar month)
{
    uchar digits = month % 10;
    uchar tens = month / 10;    
    uchar encoded = digits;
    if(tens == 1) encoded |= (tens << 4);
    WriteByte(&encoded);
}

static void WriteDay(uchar day)
{
    WriteByte(&day);    
}

static void WriteYear(uchar year)
{
    uchar digits = year % 10;    
    uchar tens = year / 10;
    uchar encoded = digits;    
    encoded |= (tens << 4);
    WriteByte(&encoded);    
}


//Read Operations

static void ReadSeconds()
{    
    uchar temp = AssembleByte();
    uchar tens = (temp & 0x70) >> 4;    
    uchar digits = (temp & 0x0F);        
    g_rawClock.secondDigits = digits;
    g_rawClock.secondsTens = tens;
    g_clock.seconds = (tens * 10) + digits;
}

static void ReadMinutes()
{    
    uchar temp = AssembleByte();    
    uchar tens = (temp & 0x70) >> 4;
    uchar digits = (temp & 0x0F);    
    g_rawClock.minutesDigits = digits;
    g_rawClock.minutesTens = tens;
    g_clock.minutes = (tens * 10) + digits;
}

static void ReadHours()
{
    //TODO: double check assumptions made here based of bit defitions from the datasheet.   
    uchar temp = AssembleByte();    
    uchar digits = (temp & 0x0F);
    //assume 1 means 24 hr clock i.e. after 12am.
    uchar tens = 0;

    tens = IsBitSet(&temp, 4);

    if(IsBitSet(&temp, 7))
    {
        tens++;
    }    

    g_rawClock.hoursDigits = digits;
    g_rawClock.hoursTens = tens;
    g_clock.minutes = (tens * 10) + digits;
}

static void ReadDate()
{
    uchar temp = AssembleByte();
    uchar tens = (temp & 0x30) >> 4;
    uchar digits = (temp & 0x0F);
    g_clock.date = (tens * 10) + digits;    
}

static void ReadMonth()
{    
    uchar temp = AssembleByte();  
    uchar digits = (temp & 0x0F);
    uchar tens = (temp & 0x10) >> 4;
    g_clock.month = (tens * 10) + digits;
}

static void ReadDay()
{    
    uchar temp = AssembleByte();
    g_clock.day = temp;
}

static void ReadYear()
{
    uchar temp = AssembleByte(); 
    uchar tens = (temp & 0xF0) >> 4;
    uchar digits = (temp & 0x0F);
    g_clock.year = (tens * 10) + digits;
}
