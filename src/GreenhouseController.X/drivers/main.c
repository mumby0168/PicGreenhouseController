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
#include "Thermometer.h"
#include "../fst.h"

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
    Fst_Init();
}


#define DEV

#ifdef DEV

int main()
{
    Init();
    Lcd_SetDisplayMode(true, false, false);        
    
    while(1)
    {         
        Lcd_ClearDisplay();
        Fst_Update();
    }
    
    return 0;    
}



#endif





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
    
    
    Init();
    
    Lcd_SetDisplayMode(true, false, false);
    
    Timing_SetTime(12, 24, 45);
    
    while(1)
    {        
        Lcd_ClearDisplay();
        
        //Timer
        Timing_ReadTime();       
        Lcd_WriteNumber(g_rawClock.hoursTens);
        Lcd_WriteNumber(g_rawClock.hoursDigits);
        
        Lcd_WriteNumber(g_rawClock.minutesTens);
        Lcd_WriteNumber(g_rawClock.minutesDigits);
        
        Lcd_WriteNumber(g_rawClock.secondsTens);
        Lcd_WriteNumber(g_rawClock.secondDigits);                       
        Lcd_SetDisplayMode(true, false, false);
        
        //Therm
        Lcd_SetCursorPosition(1, 2);
        
        if (Thermometer_ProcessTemperature() != 0)
        {
            Lcd_WriteString("Failed to process temperature.");
            continue;
        }
        
        DelayMilliSeconds(750);

        Thermometer_ScratchPad scratchPad;
        if (Thermometer_ReadScratchPad(&scratchPad, 2) != 0)
        {
            Lcd_WriteString("Failed to read scratch pad.");
            continue;
        }
        
        Thermometer_BcdTemperature temperatureBcdValue;
        Thermometer_ConvertTempratureToBcd(scratchPad.byTempMsb, scratchPad.byTempLsb, &temperatureBcdValue);        
        PrintTemperatureBcdValue(&temperatureBcdValue);
        
        DelayMilliSeconds(200);
    }
    
    return;
}

#endif