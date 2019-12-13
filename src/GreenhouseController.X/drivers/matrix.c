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
    // checks to see if the input is valid
    if (col > 3) 
        col = 3;
    
    // clears port C so data is new
    PORTC = 0x00; 
    // drives the columns pin high
    SetBitHigh(&PORTC, 3 - col); 
    // gets the state data from PORTC  
    ushort nibble = GetHighNibble(&PORTC);         
    // shifts the state into the positon for the column in the data structure.
    Matrix_usKeyState |= (nibble << (col * 4)); 
}

void Matrix_CheckColumns()
{
    //loops over all columns checking them
    for (uchar i = 0; i < 4; i++)
    {
        // clear portC
        PORTC = 0x00;
        //sets the correct bit high for the column to check.
        SetBitHigh(&PORTC, 3 - i);
        //pushes the state for the column into the right portion of bits.
        Matrix_usKeyState |= GetHighNibble(&PORTC) << (i * 4);
    }
}

// Get the portion of bits for the column passsed.
uchar Matrix_GetColumn(uchar col)
{
    return (Matrix_usKeyState >> (col * 4)) & 0x0F;
}
