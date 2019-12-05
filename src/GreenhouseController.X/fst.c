#include "fst.h"
#include <xc.h>
#include "drivers/lcd.h"
#include "drivers/matrix.h"
#include "displays/main_display.h"
#include "displays/settings_display.h"
#include "drivers/timing-chip.h"
#include "displays/settings_display.h"
#include "displays/trigger-opts.h"
#include "displays/time_set.h"
#include "displays/date_set.h"
#include "displays/temp_set.h"

/*
STATE	            0	    1	    2	    3	    4	    5	    6	    7	8	    9			    ACTION
EVENT		        BACK	SAVE	MENU1	MENU2	MENU3	LEFT	RIGHT	UP	DOWN	SETTINGS		0	DO NOTHING
0	INITIALISE	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        1	LOAD DAY TIME SETTINGS
1	MAIN	        -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    2/0		        2	LOAD NIGHT TIME SETTINGS
2	SETTINGS	    1/0	    2/0	    4/1	    4/2	    3/0	    -/0	    -/0	    -/0	-/0	    1/0		        3	LOAD HOT SETTINGS
3	CLOCK SETTINGS	2/0	    3/0	    6/0	    7/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        4	LOAD COLD SETTINGS
4	TRIGGER OPTIONS	2/0	    4/0	    5/3	    5/4	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        5	MOVE CURSOR LEFT
5	TEMP ALARM SET	4/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        6	MOVE CURSOR RIGHT
6	TIME SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        7	INCREMENT DIGIT
7	DATE SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        8	DECREMENT DIGIT
														
STATE/ACTION													
- MEANS REMAIN IN SAME STATE. So the table below sets it the the current state
1/2 -> 0x12												
**/

static uchar g_ubyFstTable[8][10] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20 }, //1
    { 0x10, 0x20, 0x41, 0x22, 0x30, 0x20, 0x20, 0x20, 0x20, 0x10 }, //2
    { 0x20, 0x30, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }, //3
    { 0x20, 0x40, 0x53, 0x44, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 }, //4
    { 0x40, 0x10, 0x50, 0x50, 0x50, 0x55, 0x56, 0x57, 0x58, 0x50 }, //5
    { 0x30, 0x10, 0x60, 0x60, 0x60, 0x65, 0x66, 0x67, 0x68, 0x60 }, //6
    { 0x30, 0x10, 0x70, 0x70, 0x70, 0x75, 0x76, 0x77, 0x78, 0x70 }  //7
};

inline void Execute(uchar pAction);


void Fst_Init(void) {
    g_fstState = 1;
    //Main_Init();
    //Timing_SetCalendar(4, 10, 10, 19);
}

uchar Fst_GetState(const uchar ubyFstValue)
{
    return (ubyFstValue & 0xF0) >> 4;
}

uchar Fst_GetAction(const uchar ubyFstValue)
{
    return (ubyFstValue & 0x0F);
}

inline void WriteCurrentState()
{
    switch(g_fstState)
    {
        case 0: 
            Lcd_WriteString("INIT");
            return;     
        case 1: 
            Lcd_WriteString("MAIN");
            return;     
        case 2: 
            Lcd_WriteString("SETTINGS");
            return;     
        case 3: 
            Lcd_WriteString("CLOCK SETTINGS");
            return;     
        case 4: 
            Lcd_WriteString("TRIGGER OPTS");
            return;     
        case 5: 
            Lcd_WriteString("TRIGG A SET");
            return;     
        case 6: 
            Lcd_WriteString("TIME SET");
            return; 
        case 7: 
            Lcd_WriteString("DATE SET");
            return;  
    }        
    
}



void Fst_Update(void)
{    
    
    g_keyState = 0;
    
    Matrix_CheckColumnState(0);
    Matrix_CheckColumnState(1);
    Matrix_CheckColumnState(2);
    Matrix_CheckColumnState(3);
    
    /*
     *COL ORDER AS LOOKING AT LCD:3 2 1 0      
     * ROW ORDER TOP->BOTTOM 8 4 2 1
     */

    uchar col1State = Matrix_GetColumn(0);
    uchar col2State = Matrix_GetColumn(1);
    uchar col3State = Matrix_GetColumn(2);
    uchar col4State = Matrix_GetColumn(3);
    
    /*Lcd_WriteNumber(col1State);
    Lcd_WriteNumber(col2State);
    Lcd_WriteNumber(col3State);
    Lcd_WriteNumber(col4State);
     */
    
    if (col4State == 8) // MENU 1
    {
        Execute(2);
    }
    else if (col4State == 4) // MENU 2
    {
        Execute(3);
    }
    else if (col4State == 2) // MENU 3
    {
        Execute(4);
    }
    else if (col4State == 1) // BACK
    {
        Execute(0);
    }
    else if(col3State == 2) // LEFT
    {
        Execute(5);
    }
    else if(col2State == 8) // UP
    {
        Execute(7);
    }
    else if(col2State == 2) // DOWN
    {
        Execute(8);
    }
    else if(col1State == 8) // SETTINGS
    {
        Execute(9);
    }
    else if(col1State == 4) // RIGHT
    {
        Execute(6);
    }
    else if(col1State == 1) // BACK
    {
        Execute(1);
    }
    
    WriteCurrentState();
    #ifdef KEYS
    
    if(Matrix_IsButtonPressed(col4State, 3) == 1)    // MENU 1
        Execute(2);
    else if(Matrix_IsButtonPressed(col4State, 2) == 1) // MENU 2
        Execute(3);
    else if(Matrix_IsButtonPressed(col4State, 1) == 1) // MENU 3
        Execute(4);
    else if(Matrix_IsButtonPressed(col4State, 0) == 1) // BACK
        Execute(0);
    else if(Matrix_IsButtonPressed(col3State, 1) == 1) // LEFT
        Execute(5);
    else if(Matrix_IsButtonPressed(col2State, 3) == 1) // UP
        Execute(7);
    else if(Matrix_IsButtonPressed(col2State, 1) == 1) // DOWN
        Execute(8);
    else if(Matrix_IsButtonPressed(col1State, 3) == 1) // SETTINGS
        Execute(9);    
    else if(Matrix_IsButtonPressed(col1State, 2) == 1) // RIGHT
        Execute(6);
    else if(Matrix_IsButtonPressed(col1State, 0) == 1) // SAVE
        Execute(1);   
       
#endif
       
    //Main_Update();
}




inline void Execute(uchar pAction)
{
    uchar val = g_ubyFstTable[g_fstState][pAction];
    uchar action = Fst_GetAction(val);
    g_fstState = Fst_GetState(val);
    Lcd_SetCursorPosition(12, 4);
    Lcd_WriteNumber(pAction);
    // Get and execute action fPointer    
}


/*
void Fst_FakeAction(uchar pAction)
{    
    uchar val = g_ubyFstTable[g_fstState][pAction];
    uchar action = Fst_GetAction(val);
    g_fstState = Fst_GetState(val);
    
    Lcd_WriteNumber(g_fstState);
    Lcd_WriteCharacter(58);      
    Lcd_WriteNumber(action);
}
*/



