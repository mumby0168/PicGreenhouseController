/*
 * File:   main.c
 * Author: billy
 *
 * Created on 23 November 2019, 13:34
 */

typedef unsigned char bool;
typedef unsigned char uchar;

void SetBitHigh(char* byValue, char byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}


bool IsBitSet(char byValue, char byBitOffset)
{
    return (byValue >> byBitOffset) & 1U;
}

inline void ClearLowNibble(uchar* val)
{
    *val = *val & 0xF0;
}

inline void ClearHighNibble(uchar* val)
{
    *val = *val & 0x0F;
}

inline uchar GetHighNibble(uchar* val)
{
    return (*val & 0xF0) >> 4;
}

#include <xc.h>
#include "lcd.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define COL_ONE_HIGH 0x01
#define COL_TWO_HIGH 0x02
#define COL_THREE_HIGH 0x04
#define COL_FOUR_HIGH 0x08

typedef struct
{
    uchar one : 1;
    uchar two : 1;
    uchar three : 1;
    uchar four: 1;
    uchar res: 4;
} Keys;

typedef struct
{
    Keys Column1;
    Keys Column2;
    Keys Column3;
    Keys Column4;
} KeyMatrix;


void Init(void);


void CheckColumn(uchar col);
void SetValues(uchar col);

KeyMatrix g_Keys;

void Init()
{
    TRISC = 0b11110000; // set first 4 as output and second 4 as input.
}


void CheckColumn(uchar col)
{        
    if (col > 3)
        col = 3;
    
    PORTC = 0x00;
    SetBitHigh(&PORTC, 3 - col);
    uchar nibble = GetHighNibble(&PORTC); 
    *((Keys*)&g_Keys + col) = *(Keys*)&nibble;;       
}

void CheckKeys(void);

void CheckKeys()
{
    int i;
    for(i = 0; i < 4; i++)
    {
        CheckColumn(i);
    }
        
}

void WriteKeysToLcd()
{
    SetCursorPosition(0, 0);
    WriteNumber(g_Keys.Column1.one);
    WriteNumber(g_Keys.Column1.two);
    WriteNumber(g_Keys.Column1.three);
    WriteNumber(g_Keys.Column1.four);
    
    WriteCharacter(58);
    
    WriteNumber(g_Keys.Column2.one);
    WriteNumber(g_Keys.Column2.two);
    WriteNumber(g_Keys.Column2.three);
    WriteNumber(g_Keys.Column2.four);
    
    SetCursorPosition(3, 0);
    
    WriteNumber(g_Keys.Column3.one);
    WriteNumber(g_Keys.Column3.two);
    WriteNumber(g_Keys.Column3.three);
    WriteNumber(g_Keys.Column3.four);
    
    WriteCharacter(58);
    
    WriteNumber(g_Keys.Column4.one);
    WriteNumber(g_Keys.Column4.two);
    WriteNumber(g_Keys.Column4.three);
    WriteNumber(g_Keys.Column4.four);
    
    
}


void main(void) {


    Init();
    Initialise();
    SetDisplayMode(true, false, false);   


    while (1)
    {
        ClearDisplay();
        CheckKeys();
        WriteKeysToLcd();        
        SetDisplayMode(true, false, false);
    }
    
}
