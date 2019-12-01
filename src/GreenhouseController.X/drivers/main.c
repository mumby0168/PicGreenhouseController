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

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

void Init()
{
    Lcd_Init();
    Timing_Init();
    Matrix_Init();    
}

void main(void) {
    
    Init();
    
    Lcd_SetDisplayMode(true, false, false);
    
    Timing_SetTime(12, 24, 45);
    
    while(1)
    {        
        Lcd_ClearDisplay();
        Timing_ReadTime();       
        Lcd_WriteNumber(g_rawClock.hoursTens);
        Lcd_WriteNumber(g_rawClock.hoursDigits);
        
        Lcd_WriteNumber(g_rawClock.minutesTens);
        Lcd_WriteNumber(g_rawClock.minutesDigits);
        
        Lcd_WriteNumber(g_rawClock.secondsTens);
        Lcd_WriteNumber(g_rawClock.secondDigits);                       
        Lcd_SetDisplayMode(true, false, false);
    }
    
    return;
}
