#include <stdio.h>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef uchar bool;

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

#define THIRTY_SIX_SECONDS 0x92
#define TWENTY_THREE_HOURS 0x92
#define TWENTY_FIVE_MINUTES 0x00

bool IsBitSet(uchar* byValue, uchar);

bool IsBitSet(uchar* byValue, uchar byBitOffset)
{
    return (*byValue >> byBitOffset) & 1U;
}


void ReadHours(uchar value)
{
    //TODO: double check assumptions made here based of bit defitions from the datasheet.   
    uchar temp = value;  
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
    g_clock.hours = (tens * 10) + digits;
}

void SetBitHigh(uchar* byValue, uchar byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}

void WriteHours(uchar hours)
{
    uchar encoded = 0;
    uchar tens = hours / 10;    
    printf("%d\n", tens);
    uchar digits = hours % 10;
    printf("%d\n", digits);
    encoded = digits;    
    if(hours > 12) 
    {
        SetBitHigh(&encoded, 7);        
    }
        
    if(hours > 9)
    {
        SetBitHigh(&encoded, 4);  
    }
    
    printf("%#010x", encoded);
}


int main()
{
    ReadHours(TWENTY_THREE_HOURS);
    printf("%d\n", g_clock.hours);
    WriteHours(10);



    return 0;
}