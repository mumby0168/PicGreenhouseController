/***********************************************************************
 * NAME: matrix.h 
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#ifndef MATRIX
#define MATRIX

#include "../libs/std.h"

// typedef struct
// {
//     uchar one : 1;
//     uchar two : 1;
//     uchar three : 1;
//     uchar four: 1;
//     uchar res: 4;
// } Keys;

// typedef struct
// {
//     Keys Column1;
//     Keys Column2;
//     Keys Column3;
//     Keys Column4;
// } KeyMatrix;

ushort g_keyState;

void Matrix_CheckColumnState(uchar col);
void Matrix_Init(void);
uchar Matrix_GetColumn(uchar col);
bool Matrix_IsButtonPressed(uchar columnState, uchar button);


// KeyMatrix g_Keys;

#endif
