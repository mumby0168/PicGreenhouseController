/*
 * File:   main.c
 * Author: billy
 *
 * Created on 23 November 2019, 13:21
 */

#include <xc.h>
#include <pic16f877a.h> 
#include "./timing-chip.h"
#include "./matrix.h"
#include "../drivers/lcd.h"
#include "../libs/std.h"
#include "../libs/Delays.h"
#include "thermometer.h"
#include "eeprom.h"
#include "../fst.h"
#include "../alarm_program.h"
#define TMR2_VAL 11
#define TMR2_TRIGGER_COUNT 12
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

void main(void) 
{   
    Lcd_Init(); //this is a pre-requisite to the fst. As is the button matrix however only the fst will use that so we leave it for the fst to handle.
    Eeprom_Load();
    Fst_Init();
    Alarm_Program_Init();
    Thermometer_Init();
    Thermometer_ProcessTemperature();
    
    Fst_ProcessEvent(FST_EVENT_INITIALISED);
    
    while(1)
    {       
        DelayMilliSeconds(75);
        Fst_Events event = Fst_Update();
        Fst_ProcessEvent(event);
        
        Thermometer_Update();        
        if (Thermometer_bProcessTemperatureComplete) //Using an ~750ms interrupt...
        {
            Alarm_Program_Update();
            Fst_ProcessEvent(FST_EVENT_PROCESS_TEMPERATURE_UPDATE);
            Thermometer_ProcessTemperature();
        }
    };
    
    return;
}