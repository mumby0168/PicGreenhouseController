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

void Init()
{
    INTCONbits.RBIE = 0;
    
    Lcd_Init();
    Timing_Init();
    Matrix_Init();
    Fst_Init();
    Thermometer_Initialise();
}


#define DEMO

#ifdef INT_DEV

int main()
{
    Lcd_Init();
    Lcd_SetDisplayMode(true, false, false); 
    Thermometer_Initialise();
    
    Lcd_WriteString("Start");
    
    char i = 0;
    Thermometer_ProcessTemperature();
    while (true)
    {
        if (g_ProcessTemperatureComplete)
        {
            Lcd_SetCursorPosition(1, 4);
            Lcd_ClearDisplay();
            Lcd_WriteCharacter(i + 48);
            i++;
            if (i > 9)
                i = 0;
            Lcd_WriteString("Temp");
            Thermometer_ProcessTemperature();
        }
    }
}

#endif

#ifdef DEV

int main()
{
    Init();    
    Lcd_SetDisplayMode(true, false, false);        
    Lcd_WriteString("Start");
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
    
    Eeprom_Load();
    
    if (g_Settings.temp > 9)
        g_Settings.temp = 0;
    
    Lcd_SetCursorPosition(1, 2);
    g_Settings.temp = ++g_Settings.temp;
    if (Eeprom_Save() == false)
    {
        Lcd_WriteString("Failed to save the eeprom.");
        DelaySeconds(10);
        return;
    }
    
    g_Settings.temp = 0x00;
    
    Thermometer_ProcessTemperature();
    char i = 0;
    while(1)
    {        
        Lcd_ClearDisplay();
        
        //Timer
        Timing_ReadTime();       
        Lcd_WriteNumber(g_rawClock.hoursTens);
        Lcd_WriteNumber(g_rawClock.hoursDigits);
        
        Lcd_WriteCharacter(':');
        
        Lcd_WriteNumber(g_rawClock.minutesTens);
        Lcd_WriteNumber(g_rawClock.minutesDigits);
        
        Lcd_WriteCharacter(':');
        
        Lcd_WriteNumber(g_rawClock.secondsTens);
        Lcd_WriteNumber(g_rawClock.secondDigits);
        
        g_Settings.temp = 0x00;
        Eeprom_Load();
        Lcd_SetCursorPosition(1, 2);
        Lcd_WriteCharacter(g_Settings.temp + 48);
        
        DelayMilliSeconds(200);
        
        if (g_ProcessTemperatureComplete)
        {
            Thermometer_ScratchPad sp;
            Thermometer_ReadScratchPad(&sp, 2);
            Thermometer_BcdTemperature bcdTemp;
            Thermometer_ConvertTempratureToBcd(sp.byTempMsb, sp.byTempLsb, &bcdTemp);
            
            Lcd_SetCursorPosition(1, 3);
            PrintTemperatureBcdValue(&bcdTemp);
            
            i++;
            if (i > 9)
                i = 0;

            Thermometer_ProcessTemperature();
        }
    }
    
    return;
}

#endif