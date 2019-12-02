#ifndef FST
#define FST
#include "libs/std.h"

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

FstStates g_fstState;

typedef void (*ActionDelegate)(void*);

void Fst_Init(void);

uchar Fst_GetState(const uchar ubyFstValue);

uchar Fst_GetAction(const uchar ubyFstValue);

void Fst_FakeAction(uchar action);

#endif