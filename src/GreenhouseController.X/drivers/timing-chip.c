/***********************************************************************
 * NAME: timing-chip.c
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#include "timing-chip.h"
#include "../libs/BinaryUtillities.h"
#include <xc.h>
#include "../libs/std.h"

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

//Structures
static RawClock rawClock;
static Clock clock;

//Write Operations
static void write_date(uchar data);
static void write_seconds(uchar seconds);
static void write_hours(uchar hours);
static void write_minutes(uchar minutes);
static void write_date(uchar date);
static void write_month(uchar month);
static void write_day(uchar day);
static void write_year(uchar year);

//Read Operations
static void read_seconds(void);
static void read_minutes(void);
static void read_hours(void);
static void read_date(void);
static void read_month(void);
static void read_day(void);
static void read_year(void);

//Low Level API
inline static void write_command_byte(uchar);
static uchar assemble_byte();
static void write_byte(uchar *);

/***********************Implementation*************************/

// High Level API

void Timing_ReadRawClock(RawClock *pRawClock)
{
    pRawClock = (RawClock *)&rawClock;
}

void Timing_ReadClock(Clock *pClock)
{
    pClock = (Clock *)&clock;
}

void Timing_Init()
{
    //Init structs
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
    write_command_byte(CONTROL_CMD);
    write_byte(0);

    COMPLETE_OPERATION // Drives reset low.
}

void Timing_SetCalendar(uchar day, uchar date, uchar month, uchar year)
{
    write_command_byte(WRITE_DAY);
    write_day(day);
    COMPLETE_OPERATION

    write_command_byte(WRITE_DATE);
    write_date(date);
    COMPLETE_OPERATION

    write_command_byte(WRITE_MONTH);
    write_month(month);
    COMPLETE_OPERATION

    write_command_byte(WRITE_YEAR);
    write_year(year);
    COMPLETE_OPERATION
}

void Timing_SetTime(uchar hours, uchar minutes, uchar seconds)
{
    write_command_byte(WRITE_HOURS);
    write_hours(hours);
    COMPLETE_OPERATION

    write_command_byte(WRITE_MINUTES);
    write_minutes(minutes);
    COMPLETE_OPERATION

    write_command_byte(WRITE_SECONDS);
    write_seconds(seconds);
    COMPLETE_OPERATION
}

void Timing_ReadTime()
{
    write_command_byte(READ_HOURS);
    read_hours();
    COMPLETE_OPERATION

    write_command_byte(READ_MINUTES);
    read_minutes();
    COMPLETE_OPERATION

    write_command_byte(READ_SECONDS);
    read_seconds();
    COMPLETE_OPERATION
}

void Timing_ReadCalendar()
{
    write_command_byte(READ_DAY);
    read_day();
    COMPLETE_OPERATION

    write_command_byte(READ_DATE);
    read_date();
    COMPLETE_OPERATION

    write_command_byte(READ_MONTH);
    read_month();
    COMPLETE_OPERATION

    write_command_byte(READ_YEAR);
    read_year();
    COMPLETE_OPERATION
}

//Guidance from here https://www.mathsisfun.com/leap-years.html
bool Timing_IsLeapYear(const uchar year)
{
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        return true;

    return false;
}

//Following the key value method from this website http://mathforum.org/dr.math/faq/faq.calendar.html
static uchar s_aubyMonthKeyValue[12] = {1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
static uchar s_aubyDayOfWeekMap[7] = {6, 7, 1, 2, 3, 4, 5}; // From website: This time, 1 means Sunday, 2 means Monday,
uchar Timing_GetDayFromDate(const uchar date, const uchar month, const uchar year)
{
    uchar a = year / 4;
    a += date;
    a += s_aubyMonthKeyValue[month - 1];

    if (Timing_IsLeapYear(year) && (month == 1 || month == 2))
    {
        a -= 1;
    }

    a += 6; //assuming 2000's as no opt to specify first two digits
    a += year;

    a %= 7;

    return s_aubyDayOfWeekMap[a];
}

//Low Level API
static inline void write_command_byte(uchar byte)
{
    //set io pin to output
    IO_CONFIG = 0;
    RST = 1; // Drive RST High
    //write the 8 bits of the command byte.
    write_byte(&byte);
}

static void write_byte(uchar *byte)
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

static inline uchar assemble_byte()
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
    return (ret);
}

//Write Operations

static void write_seconds(uchar seconds)
{
    uchar encoded = 0;
    uchar tens = seconds / 10;
    uchar digits = seconds % 10;
    encoded = digits;
    encoded |= (tens << 4);
    write_byte(&encoded);
}

static void write_minutes(uchar minutes)
{
    uchar encoded = 0;
    uchar tens = minutes / 10;
    uchar digits = minutes % 10;
    encoded = digits;
    encoded |= (tens << 4);
    write_byte(&encoded);
}

static void write_hours(uchar hours)
{
    uchar encoded = 0;
    uchar tens = hours / 10;

    uchar digits = hours % 10;
    encoded = digits;

    encoded |= (tens << 4);

    write_byte(&encoded);
}

static void write_date(uchar date)
{
    uchar digits = date % 10;
    uchar tens = date / 10;
    uchar encoded = digits;
    encoded |= (tens << 4);
    write_byte(&encoded);
}

static void write_month(uchar month)
{
    uchar digits = month % 10;
    uchar tens = month / 10;
    uchar encoded = digits;
    if (tens == 1)
        encoded |= (tens << 4);
    write_byte(&encoded);
}

static void write_day(uchar day)
{
    write_byte(&day);
}

static void write_year(uchar year)
{
    uchar digits = year % 10;
    uchar tens = year / 10;
    uchar encoded = digits;
    encoded |= (tens << 4);
    write_byte(&encoded);
}

//Read Operations

static void read_seconds()
{
    uchar temp = assemble_byte();
    uchar tens = (temp & 0x70) >> 4;
    uchar digits = (temp & 0x0F);
    rawClock.ucSecondsDigits = digits;
    rawClock.ucSecondsTens = tens;
    clock.ucSeconds = (tens * 10) + digits;
}

static void read_minutes()
{
    uchar temp = assemble_byte();
    uchar tens = (temp & 0x70) >> 4;
    uchar digits = (temp & 0x0F);
    rawClock.ucMinutesDigits = digits;
    rawClock.ucMinutesTens = tens;
    clock.ucMinutes = (tens * 10) + digits;
}

static void read_hours()
{    
    uchar temp = assemble_byte();
    uchar digits = (temp & 0x0F);        
    uchar tens = (temp & 0x30) >> 4;    

    rawClock.ucHoursDigits = digits;
    rawClock.ucHoursTens = tens;
    clock.ucHours = (tens * 10) + digits;
}

static void read_date()
{
    uchar temp = assemble_byte();
    uchar tens = (temp & 0x30) >> 4;
    uchar digits = (temp & 0x0F);
    clock.ucDate = (tens * 10) + digits;
}

static void read_month()
{
    uchar temp = assemble_byte();
    uchar digits = (temp & 0x0F);
    uchar tens = (temp & 0x10) >> 4;
    clock.ucMonth = (tens * 10) + digits;
}

static void read_day()
{
    uchar temp = assemble_byte();
    clock.ucDay = temp;
}

static void read_year()
{
    uchar temp = assemble_byte();
    uchar tens = (temp & 0xF0) >> 4;
    uchar digits = (temp & 0x0F);
    clock.ucYear = (tens * 10) + digits;
}
