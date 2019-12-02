#include <xc.h> 
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#include "matrix.h"
#include "std.h"


#define nullptr 0


uchar GetState(const uchar ubyFstValue)
{
    return (ubyFstValue & 0xF0) >> 4;
}

uchar GetAction(const uchar ubyFstValue)
{
    return (ubyFstValue & 0x0F);
}

/*
STATE	            0	    1	    2	    3	    4	    5	    6	    7	8	    9			    ACTION
EVENT		        BACK	SAVE	MENU1	MENU2	MENU3	LEFT	RIGHT	UP	DOWN	SETTINGS		0	DO NOTHING
0	INITIALISE	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        1	LOAD DAY TIME SETTINGS
1	MAIN	        -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	    -/0	-/0	    2/0		        2	LOAD NIGHT TIME SETTINGS
2	SETTINGS	    1/0	    2/0	    4/1	    4/2	    3/0	    -/0	    -/0	    -/0	-/0	    1/0		        3	LOAD HOT SETTINGS
3	CLOCK SETTINGS	1/0	    3/0	    6/0	    7/0	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        4	LOAD COLD SETTINGS
4	TRIGGER OPTIONS	2/0	    4/0	    5/3	    5/4	    -/0	    -/0	    -/0	    -/0	-/0	    -/0		        5	MOVE CURSOR LEFT
5	TEMP ALARM SET	4/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        6	MOVE CURSOR RIGHT
6	TIME SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        7	INCREMENT DIGIT
7	DATE SET	    3/0	    1/0	    -/0	    -/0	    -/0	    -/5	    -/6	    -/7	-/8	    -/0		        8	DECREMENT DIGIT
														
STATE/ACTION													
"-" MEANS REMAIN IN SAME STATE. So the table below sets it the the current state
1/2 -> 0x12												
*/

typedef enum fstEvents
{
    FST_EVENT_BACK_BUTTON = 0,
    FST_EVENT_SAVE_BUTTON = 1,
    FST_EVENT_MENU_1_BUTTON = 2,
    FST_EVENT_MENU_2_BUTTON = 3,
    FST_EVENT_MENU_3_BUTTON = 4,
    FST_EVENT_LEFT_BUTTON = 5,
    FST_EVENT_RIGHT_BUTTON = 6,
    FST_EVENT_UP_BUTTON = 7,
    FST_EVENT_DOWN_BUTTON = 8,
    FST_EVENT_SETTINGS_BUTTON = 9
} FstEvents;

typedef enum fstStates
{
    FST_STATES_INITIALISE = 0,
    FST_STATES_MAIN = 1,
    FST_STATES_SETTINGS = 2,
    FST_STATES_CLOCK_SETTINGS = 3,
    FST_STATES_TRIGGER_OPTIONS = 4,
    FST_STATES_TEMP_ALARM_SET = 5,
    FST_STATES_TIME_SET = 6,
    FST_STATES_DATE_SET = 7 
} FstStates;

FstStates g_fstState = 1;


uchar g_ubyFstTable[8][10] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 }, //0
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20 }, //1
    { 0x10, 0x20, 0x41, 0x22, 0x30, 0x20, 0x20, 0x20, 0x20, 0x10 }, //2
    { 0x10, 0x30, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }, //3
    { 0x20, 0x40, 0x53, 0x44, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 }, //4
    { 0x40, 0x10, 0x50, 0x50, 0x50, 0x55, 0x56, 0x57, 0x58, 0x50 }, //5
    { 0x30, 0x10, 0x60, 0x60, 0x60, 0x65, 0x66, 0x67, 0x68, 0x60 }, //6
    { 0x30, 0x10, 0x70, 0x70, 0x70, 0x75, 0x76, 0x77, 0x78, 0x70 }  //7
};

typedef void (*ActionDelegate)(void*);

void test(void* p)
{
    
}

ActionDelegate g_pFstActions[9] = { nullptr, &test, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };



void main(void) {
    
    Matrix_Init();           
    while(1)
    {
        
    }
}