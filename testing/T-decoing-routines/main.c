#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef uchar bool;

void BinaryPrint(char  byte)
{
    printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
    printf("\n");
}

void HexPrint(uchar byte)
{
    printf("%x\n", byte);
}

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


bool IsBitSet(uchar* byValue, uchar);

bool IsBitSet(uchar* byValue, uchar byBitOffset)
{
    return (*byValue >> byBitOffset) & 1U;
}


void SetBitHigh(uchar* byValue, uchar byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}



// void WriteDate(uchar date)
// {
//     uchar digits = date % 10;
//     uchar tens = date / 10;
//     printf("%d", tens);
//     uchar encoded = digits;
//     encoded |= (tens << 4);
//     WriteByte(&encoded);    
// }

// void WriteMonth(uchar month)
// {
//     uchar digits = month % 10;
//     uchar tens = month / 10;    
//     uchar encoded = digits;
//     if(tens == 1) encoded |= (tens << 4);
//     WriteByte(&encoded);
// }

// void WriteDay(uchar day)
// {
//     WriteByte(&day);    
// }

// void WriteYear(uchar year)
// {
//     uchar digits = year % 10;    
//     uchar tens = year / 10;
//     uchar encoded = digits;    
//     encoded |= (tens << 4);
//     WriteByte(&encoded);    
// }

void WriteYear(uchar year)
{
    uchar digits = year % 10;    
    uchar tens = year / 10;
    uchar encoded = digits;    
    encoded |= (tens << 4);
    BinaryPrint(encoded); 
}

void ReadYear()
{
    uchar temp = 0x96; 
    uchar tens = (temp & 0xF0) >> 4;
    uchar digits = (temp & 0x0F);
    g_clock.year = (tens * 10) + digits;
}

int main()
{        
    WriteYear(96);
    ReadYear();
    printf("%d\n", g_clock.year);
    return 0;
}