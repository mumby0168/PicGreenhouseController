/***********************************************************************
 * NAME: timing-chip.h 
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#ifndef TIMING
#define TIMING

typedef unsigned char uchar;

typedef struct
{
    uchar seconds;
    uchar minutes;
    uchar hours;
    uchar date;
    uchar day;
    uchar month;
    uchar year;
} Clock;

typedef struct 
{
    uchar secondsTens;
    uchar secondDigits;
    uchar minutesTens;
    uchar minutesDigits;
    uchar hoursTens;
    uchar hoursDigits;

} RawClock;


Clock g_clock;
RawClock g_rawClock;


void Timing_SetTime(uchar hours, uchar minutes, uchar seconds);
void Timing_SetCalendar(uchar day, uchar date, uchar month, uchar year);
void Timing_ReadTime(void);
void Timing_ReadCalendar(void);
void Timing_Init(void);

#endif