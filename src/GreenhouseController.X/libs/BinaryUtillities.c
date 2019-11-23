
#include "BinaryUtillities.h"

inline void SetBitHigh(uchar* byValue, uchar byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}

inline void SetBitLow(uchar* byValue, uchar byBitOffset)
{
    *byValue &= ~(1U << byBitOffset);
}

void SetBit(uchar* byValue, uchar byBitOffset, uchar byBitValue)
{
    if (byBitValue > 0)
        SetBitHigh(byValue, byBitOffset);
    else
        SetBitLow(byValue, byBitOffset);
}

inline bool IsBitSet(uchar* byValue, uchar byBitOffset)
{
    return (*byValue >> byBitOffset) & 1U;
}

inline void ClearLowNibble(uchar* val)
{
    *val = *val & 0xF0;
}

inline void ClearHighNibble(uchar* val)
{
    *val = *val & 0x0F;
}

inline uchar GetLowNibble(uchar* val)
{
    return *val & 0xF0;
}

inline uchar GetHighNibble(uchar* val)
{
    return (*val & 0xF0) >> 4;
}

inline void SetLowNibble(uchar* val, uchar newVal)
{
    *val = (*val & 0xF0) | newVal;
}

inline void SetHighNibble(uchar* val, uchar newVal)
{
    *val = (*val & 0x0F) | newVal << 4;
}
