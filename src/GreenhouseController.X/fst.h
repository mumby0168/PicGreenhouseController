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
    FST_EVENT_SETTINGS_BUTTON = 9,
    FST_EVENT_INITIALISED = 10,
    FST_EVENT_PROCESS_TEMPERATURE_UPDATE = 11,
    FST_EVENT_NO_EVENT = 0xFF
} Fst_Events;

typedef enum fstStates
{
    FST_STATE_INITIALISING = 0,
    FST_STATE_MAIN_SCREEN = 1,
    FST_STATE_SETTINGS_SCREEN = 2,
    FST_STATE_CLOCK_SETTINGS_SCREEN = 3,
    FST_STATE_TRIGGER_OPTIONS_SCREEN = 4,
    FST_STATE_TEMP_ALARM_SET_SCREEN = 5,
    FST_STATE_TIME_SET_SCREEN = 6,
    FST_STATE_DATE_SET_SCREEN = 7
} Fst_States;

typedef enum fstActions
{
    FST_ACTION_DO_NOTHING = 0,
    FST_ACTION_LOAD_DAY_SETTINGS = 1,
    FST_ACTION_LOAD_NIGHT_SETTINGS = 2,
    FST_ACTION_HOT_SETTINGS = 3,
    FST_ACTION_COLD_SETTINGS = 4,
    FST_ACTION_HANDLE_LEFT_BUTTON = 5,
    FST_ACTION_HANDLE_RIGHT_BUTTON = 6,
    FST_ACTION_HANDLE_UP_BUTTON = 7,
    FST_ACTION_HANDLE_DOWN_BUTTON = 8,
    FST_ACTION_REDRAW_TEMPERATURE = 9
} Fst_Actions;

typedef void (*Fst_ActionDelegate)();
bool Fst_SetAction(uchar ubyActionNumber, Fst_ActionDelegate pDelegate);
bool Fst_ClearAction(uchar ubyActionNumber);

void Fst_Init(void);
void Fst_ProcessEvent(Fst_Events event);
Fst_Events Fst_Update(void);

#endif