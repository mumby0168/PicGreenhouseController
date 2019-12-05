/***********************************************************************
 * NAME: matrix.h 
 * PURPOSE: provide the api available for the timing chip
 * AUTHOR: B.Mumby
***********************************************************************/
#ifndef MATRIX
#define MATRIX

#include "../libs/std.h"

ushort Matrix_usKeyState;

void Matrix_CheckColumnState(uchar col);
void Matrix_Init(void);
uchar Matrix_GetColumn(uchar col);
bool Matrix_IsButtonPressed(uchar columnState, uchar button);

#endif
