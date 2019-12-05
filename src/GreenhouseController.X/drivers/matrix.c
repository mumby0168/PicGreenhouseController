/***********************************************************************
 * NAME: therm.c
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#include <xc.h>
#include "matrix.h"
#include "../libs/BinaryUtillities.h"

void Matrix_Init()
{
    TRISC = 0b11110000; // set first 4 as output and second 4 as input.
}

void Matrix_CheckColumnState(uchar col)
{        
    if (col > 3)
        col = 3;
    
    PORTC = 0x00;
    SetBitHigh(&PORTC, 3 - col);
    ushort nibble = GetHighNibble(&PORTC);          
    Matrix_usKeyState |= (nibble << (col * 4));
}

uchar Matrix_GetColumn(uchar col)
{
    uchar keySection = (Matrix_usKeyState >> (col * 4)) & 0x0F;
    return keySection;
}

bool Matrix_IsButtonPressed(uchar columnState, uchar button)
{
    bool isSet = columnState & (1 << button);
    return isSet;    
}



