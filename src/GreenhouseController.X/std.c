#include "libs/std.h"

void memcpy(uchar* const pTarget, const uchar* const pSource, const uchar ubySize)
{
    for (uchar i = 0; i < ubySize; i++)
        pTarget[i] = pSource[i];
}