/*
 * File:   main.c
 * Author: billy
 *
 * Created on 23 November 2019, 13:21
 */

#include <xc.h> 
#include "./timing-chip.h"
#include "./matrix.h"
#include "../drivers/lcd.h"
#include "../libs/std.h"
#include "../libs/Delays.h"
#include "thermometer.h"
#include "eeprom.h"
#include "../fst.h"
#define TMR2_VAL 11
#define TMR2_TRIGGER_COUNT 12
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define DEMO

#ifdef DEMO
void PrintTemperatureBcdValue(Thermometer_BcdTemperature* temperatureBcdValue)
{
    if (temperatureBcdValue->bIsNegative)
        Lcd_WriteCharacter('-');

    Lcd_WriteCharacter(temperatureBcdValue->ubyHundreds + 48);
    Lcd_WriteCharacter(temperatureBcdValue->ubyTens + 48);
    Lcd_WriteCharacter(temperatureBcdValue->ubyUnits + 48);
    Lcd_WriteCharacter('.');
    Lcd_WriteCharacter(temperatureBcdValue->ubyTenths + 48);
    Lcd_WriteCharacter(temperatureBcdValue->ubyHundredths + 48);
    Lcd_WriteCharacter(temperatureBcdValue->ubyThousandths + 48);
    Lcd_WriteCharacter(temperatureBcdValue->ubyTenThousandths + 48);
} 

void main(void) 
{   
    Lcd_Init(); //this is a pre-requisite to the fst. As is the button matrix however only the fst will use that so we leave it for the fst to handle.
    Fst_Init();
//    Thermometer_Init();
//    Thermometer_ProcessTemperature();

    Fst_ProcessEvent(FST_EVENT_INITIALISED);
    uchar s_ubyInterruptTriggerCount = 0;
    while(1)
    {       
        DelayMilliSeconds(150);
        Fst_Events event = Fst_Update();
        Fst_ProcessEvent(event);
        
        Fst_ProcessEvent(FST_EVENT_PROCESS_TEMPERATURE_UPDATE);
//        if (PIR1bits.TMR2IF == 1)
//        {
//            s_ubyInterruptTriggerCount++;
//            if (s_ubyInterruptTriggerCount > TMR2_TRIGGER_COUNT)
//            {
//                T2CONbits.TMR2ON = 0; //turn off the timer until the next time process temp is called...
//                Thermometer_bProcessTemperatureComplete = true;
//                s_ubyInterruptTriggerCount = 0;
//            }
//
//            TMR2 = TMR2_VAL;
//            PIR1bits.TMR2IF = 0;
//        }
//        
//        if (Thermometer_bProcessTemperatureComplete)
//        {
//            Fst_ProcessEvent(FST_EVENT_PROCESS_TEMPERATURE_UPDATE);
//            Thermometer_ProcessTemperature();
//        }
    };
    
    return;
}

#endif