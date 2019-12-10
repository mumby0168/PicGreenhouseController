/***********************************************************************
 * NAME: timing-chip.h 
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#ifndef TIMING
#define TIMING

#include "../libs/std.h"

typedef struct
{
    uchar ucSeconds;
    uchar ucMinutes;
    uchar ucHours;
    uchar ucDay;
    uchar ucDate;
    uchar ucMonth;
    uchar ucYear;
} Clock;

typedef struct 
{
    uchar ucHoursTens;
    uchar ucHoursDigits;
    uchar ucMinutesTens;
    uchar ucMinutesDigits;
    uchar ucSecondsTens;
    uchar ucSecondsDigits;    
} RawClock;

const uchar g_aubyDaysInMonths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


void Timing_ReadRawClock(RawClock* pRawClock);
void Timing_ReadClock(Clock* pclock);
void Timing_SetTime(uchar ucHours, uchar ucMinutes, uchar ucSeconds);
void Timing_SetCalendar(uchar ucDay, uchar ucDate, uchar ucMonth, uchar ucYear);
void Timing_ReadTime(void);
void Timing_ReadCalendar(void);
void Timing_Init(void);
bool Timing_IsLeapYear(const uchar ucYear);
uchar Timing_GetDayFromDate(const uchar ucDate, const uchar ucMonth, const uchar ucYear);

#endif