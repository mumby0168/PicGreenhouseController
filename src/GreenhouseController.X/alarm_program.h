#ifndef ALARM_PROGRAM_H
#define	ALARM_PROGRAM_H

#include "libs/std.h"

typedef enum _alarm_program_programs
{
    ALARM_PROGRAM_DAY = 1,
    ALARM_PROGRAM_NIGHT = 2
} Alarm_Program_Programs;

void Alarm_Program_Init(void);
void Alarm_Program_Update(void);
Alarm_Program_Programs Alarm_Program_GetProgram(void);
bool Alarm_Program_IsHeating(void);
bool Alarm_Program_IsCooling(void);

#endif

