#include "lcd.h"
#include <xc.h> 

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

typedef struct
{
    uchar char1;
    uchar char2;
    uchar char3;
    uchar char4;    
    
} MenuItem;


const uchar menuSize = 2;
MenuItem items[2];

void SetupMenu()
{    
    MenuItem item1;
    item1.char1 = 'h';
    item1.char2 = 'o';
    item1.char3 = 'm';
    item1.char4 = 'e';
            
    MenuItem item2;
    item2.char1 = 'o';
    item2.char2 = 'p';
    item2.char3 = 't';
    item2.char4 = 's';
    
    items[0] = item1;
    items[1] = item2;
}

uchar counter = 0;

void DrawMenu()
{
    WriteCharacter(91);
    
    uchar i;
    for(i = 0; i < menuSize; i++)
    {
        WriteCharacter(items[i].char1);
        WriteCharacter(items[i].char2);
        WriteCharacter(items[i].char3);
        WriteCharacter(items[i].char4);            
    }
    
    WriteCharacter(93);
}

void IncrementMenu()
{
    if(counter == menuSize - 1) counter = 0;
    else counter++;
}



void main(void)
{
    Initialise();
    
    SetDisplayResolution(false, true);
    SetDisplayMode(true, false, false);
    
    while(1)
    {
        DrawMenu();
        IncrementMenu();
    }
}

