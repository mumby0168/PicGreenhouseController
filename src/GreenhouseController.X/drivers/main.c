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

void action_1()
{
    Lcd_SetCursorPosition(1, 4);
    Lcd_WriteString("Action One");
    DelaySeconds(1);
}

void main(void) 
{   
    Lcd_Init();
    Fst_Init();
    
    Fst_SetAction(1, &action_1);
    
    DelaySeconds(1);
    
    Fst_ProcessEvent(FST_EVENT_INITIALISED);
    
    while(1)
    {       
        Fst_Update();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    };
    
    return;
}

#endif