#ifndef BIT_UTILLITIES_H
#define	BIT_UTILLITIES_H

#include "std.h"

inline void SetBitHigh(uchar* byValue, uchar byBitOffset);
inline void SetBitLow(uchar* byValue, uchar byBitOffset);
void SetBit(uchar* byValue, uchar byBitOffset, uchar byBitValue);
inline bool IsBitSet(uchar* byValue, uchar byBitOffset);
inline void ClearLowNibble(uchar* val);
inline void ClearHighNibble(uchar* val);
inline uchar GetLowNibble(uchar* val);
inline uchar GetHighNibble(uchar* val);
inline void SetLowNibble(uchar* val, uchar newVal);
inline void SetHighNibble(uchar* val, uchar newVal);

#endif

