/***********************************************************************
 * NAME: matrix.h 
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#ifndef MATRIX
#define MATRIX

#include "std.h"

typedef struct
{
    uchar one : 1;
    uchar two : 1;
    uchar three : 1;
    uchar four: 1;
    uchar res: 4;
} Keys;

typedef struct
{
    Keys Column1;
    Keys Column2;
    Keys Column3;
    Keys Column4;
} KeyMatrix;

void Matrix_CheckColumn(uchar col);
void Matrix_Init(void);

KeyMatrix g_Keys;

#endif
