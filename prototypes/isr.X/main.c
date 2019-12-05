#include <xc.h>
#include "BitUtillities.h"
#include "Lcd.h"
#include "std.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define TMR2_VALUE 11

int count = 0;
volatile uchar draw = 0;

void __interrupt() timer_isr()
{  
    if(TMR2IF==1)
    {
        TMR2 = TMR2_VALUE;     /*Load the timer Value, (Note: Timervalue is 101 instead of 100 as the
                          Timer2 needs two instruction Cycles to start incrementing TMR2 */
        TMR2IF=0;       // Clear timer interrupt flag

        if(count>=12) //500us * 2000=1000000us=1sec
        {
            count=0;
            draw = 1;;   // complement the value for blinking the LEDs
        }
        else
        {
            count++;  // Keep incrementing the count till it reaches 2000 to generate 1sec delay
        }
    } 
}


void main()
{   
    Lcd_Init();
    Lcd_SetDisplayMode(true, false, false);

    T2CON = 0b01110111;  // Timer2 with external freq and 16 as prescalar
    TMR2 = TMR2_VALUE;       // Load the timer value for 500us delay
    PIR2 = 0xFF;
    TMR2IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR2ON = 1;

    while(1)
    {
        if (draw == 1)
        {
            Lcd_WriteString("Boom");
            draw = 0;
        }
    }
}