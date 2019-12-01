/***********************************************************************
 * NAME: therm.c
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/

#include "matrix.h"
#include <xc.h>
#include "../libs/BinaryUtillities.h"


void Matrix_Init()
{
    TRISC = 0b11110000; // set first 4 as output and second 4 as input.
}

void Matrix_CheckColumn(uchar col)
{        
    if (col > 3)
        col = 3;
    
    PORTC = 0x00;
    SetBitHigh(&PORTC, 3 - col);
    uchar nibble = GetHighNibble(&PORTC); 
    *((Keys*)&g_Keys + col) = *(Keys*)&nibble;;       
}

