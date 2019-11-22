#include <xc.h>
#include "std.h"
#include "Delays.h"
#include "BitUtillities.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

const char* Byte_ToString(const char byValue)
{
    char hundreds = byValue / 100;
    char tens = (byValue % 100) / 10;
    char units = (byValue % 100) % 10;
    
    return "a";
}

void main(void) {
    ADCON1 = 0x07;
    TRISB = 0x00;
    PORTB = 0xFF;

    while (true)
    {
        DelaySeconds(2);
        SetBitLow(&PORTB, 0);
        DelaySeconds(2);
        PORTB = 0xFF;
    };
    
    return;
}
