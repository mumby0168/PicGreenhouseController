#include "fst.h"
#include <xc.h>
#include "drivers/lcd.h"
#include "drivers/matrix.h"
#include "displays/main_display.h"
#include "displays/settings_display.h"
#include "drivers/timing-chip.h"
#include "displays/settings_display.h"
#include "displays/trigger_options.h"
#include "displays/time_set.h"
#include "displays/date_set.h"
#include "displays/temp_set.h"
#include "libs/Delays.h"
#include "displays/clock_settings.h"

static Fst_States s_FstState;
#define FST_NUMBER_OF_STATES 8
#define FST_NUMBER_OF_EVENTS 12
#define FST_NUMBER_OF_ACTIONS 11

/*
STATE	            0	    1	    2	    3	    4	    5	    6	    7	8	    9           10              11                  ACTION
EVENT		        BACK	SAVE	MENU1	MENU2	MENU3	LEFT	RIGHT	UP	DOWN	SETTINGS    INITIALISED     TEMP_UPDATED        0	DO NOTHING
0	INITIALISE	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		    -/0             -/0                 1	LOAD DAY TIME SETTINGS
1	MAIN	        -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    2/0		    -/0             1/9                 2	LOAD NIGHT TIME SETTINGS
2	SETTINGS	    1/0	    2/0	    4/1	    4/2	    3/0	    -/0	    -/0	    -/0	-/0	    1/0		    -/0             -/0                 3	LOAD HOT SETTINGS
3	CLOCK SETTINGS	2/0	    3/0	    6/0	    7/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		    -/0             -/0                 4	LOAD COLD SETTINGS
4	TRIGGER OPTIONS	2/0	    4/0	    5/3	    5/4	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		    -/0             -/0                 5	MOVE CURSOR LEFT
5	TEMP ALARM SET	4/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		    -/0             -/0                 6	MOVE CURSOR RIGHT
6	TIME SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		    -/0             -/0                 7	INCREMENT DIGIT
7	DATE SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		    -/0             -/0                 8	DECREMENT DIGIT
                                                                                                                                        9   REDRAW TEMP
 *                                                                                                                                      A   save
STATE/ACTION													
- MEANS REMAIN IN SAME STATE. So the table below sets it the the current state
1/2 -> 0x12												
 **/

static Fst_ActionDelegate s_FstActions[FST_NUMBER_OF_ACTIONS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

static const uchar s_ubyFstTable[FST_NUMBER_OF_STATES][FST_NUMBER_OF_EVENTS] ={
    /* BACK	SAVE  MENU1	MENU2 MENU3	LEFT  RIGHT	UP	  DOWN	SET   INIT  TEMP_UPDATED   */
    /* 0 INITIALISE */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00 },
    /* 1 MAIN */
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x19 },
    /* 2 SETTINGS */
    { 0x10, 0x20, 0x41, 0x42, 0x30, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20 },
    /* 3 CLOCK SETTINGS */
    { 0x20, 0x30, 0x60, 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 },
    /* 4 TRIGGER OPTS */
    { 0x20, 0x40, 0x54, 0x53, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 },
    /* 5 TEMP ALARM SET */
    { 0x40, 0x1A, 0x50, 0x50, 0x50, 0x55, 0x56, 0x57, 0x58, 0x50, 0x50, 0x50 },
    /* 6 TIME SET */
    { 0x30, 0x1A, 0x60, 0x60, 0x60, 0x65, 0x66, 0x67, 0x68, 0x60, 0x60, 0x60 },
    /* 7 DATE SET */
    { 0x30, 0x1A, 0x70, 0x70, 0x70, 0x75, 0x76, 0x77, 0x78, 0x70, 0x70, 0x70 }
};

void Fst_Init(void) 
{
    s_FstState = FST_STATE_INITIALISING;
    Matrix_Init();
    
//    Lcd_ClearDisplay();
//    Lcd_SetCursorPosition(1, 1);
//    Lcd_WriteString("B&R Greenhouse Temperature Controller.");
//    Lcd_SetCursorPosition(1, 4);
//    Lcd_WriteString("Initialising...");
    
    //Initialise displays that need 
    Timing_Init();
    Main_Display_Init();
    Temp_Set_Display_Init();
}

static uchar fst_get_state(const uchar ubyFstValue) 
{
    uchar ret = ubyFstValue & 0xF0;
    ret = ret >> 4;
    return ret;
}

static Fst_ActionDelegate fst_get_action(const uchar ubyFstValue) 
{
    return s_FstActions[(ubyFstValue & 0x0F)];
}

void Fst_SetAction(uchar ubyActionNumber, Fst_ActionDelegate pDelegate) 
{
    s_FstActions[ubyActionNumber] = pDelegate;
}

void Fst_ProcessEvent(Fst_Events event) 
{
    if (event == FST_EVENT_NO_EVENT)
    {
        return;
    }
    
    uchar ubyNewFstValue = s_ubyFstTable[s_FstState][event];

    Fst_States newState = fst_get_state(ubyNewFstValue);
    if (newState != s_FstState) 
    {
        s_FstState = newState;
        
        Lcd_ClearDisplay();
        Lcd_SetCursorPosition(1, 1);
       
        switch (s_FstState) 
        {
            case FST_STATE_MAIN_SCREEN:
                Main_Display();
                break;

            case FST_STATE_SETTINGS_SCREEN:
                Settings_Display();
                break;

            case FST_STATE_TRIGGER_OPTIONS_SCREEN:
                Trigger_Options_Display();
                break;

            case FST_STATE_TEMP_ALARM_SET_SCREEN:
                Temp_Set_Display();
                break;

            case FST_STATE_CLOCK_SETTINGS_SCREEN:
                Clock_Setttings_Display();
                break;

            case FST_STATE_TIME_SET_SCREEN:
                Time_Set_Display();
                break;

            case FST_STATE_DATE_SET_SCREEN:
                Date_Set_Display();
                break;

            default:
                break;
        }
    }
    
    Fst_ActionDelegate pAction = fst_get_action(ubyNewFstValue);
    if (pAction != nullptr)
        pAction();
    
    return;
}

Fst_Events Fst_Update(void) 
{
    Matrix_usKeyState = 0;

    Matrix_CheckColumnState(0);
    Matrix_CheckColumnState(1);
    Matrix_CheckColumnState(2);
    Matrix_CheckColumnState(3);

    /*
     * COL ORDER AS LOOKING AT LCD:3 2 1 0      
     * ROW ORDER TOP->BOTTOM 8 4 2 1
     */

    uchar col1State = Matrix_GetColumn(0);
    uchar col2State = Matrix_GetColumn(1);
    uchar col3State = Matrix_GetColumn(2);
    uchar col4State = Matrix_GetColumn(3);

    if (col4State == 8) // MENU 1
    {
        return (FST_EVENT_MENU_1_BUTTON);
    } else if (col4State == 4) // MENU 2
    {
        return (FST_EVENT_MENU_2_BUTTON);
    } else if (col4State == 2) // MENU 3
    {
        return (FST_EVENT_MENU_3_BUTTON);
    } else if (col4State == 1) // BACK
    {
        return (FST_EVENT_BACK_BUTTON);
    } else if (col3State == 4) // LEFT
    {
        return (FST_EVENT_LEFT_BUTTON);
    } else if (col2State == 8) // UP
    {
        return (FST_EVENT_UP_BUTTON);
    } else if (col2State == 2) // DOWN
    {
        return (FST_EVENT_DOWN_BUTTON);
    } else if (col1State == 8) // SETTINGS
    {
        return (FST_EVENT_SETTINGS_BUTTON);
    } else if (col1State == 4) // RIGHT
    {
        return (FST_EVENT_RIGHT_BUTTON);
    } else if (col1State == 1) // BACK
    {
        return (FST_EVENT_SAVE_BUTTON);
    }
    
    return FST_EVENT_NO_EVENT;
}