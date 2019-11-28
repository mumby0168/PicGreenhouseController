#include <xc.h> 
#include "Lcd.h"
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

void main(void) 
{    
    Lcd_Init();
    Lcd_SetDisplayMode(true, false, false); //Turn on display and cursor and blink
    
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteCharacter('a');
    Lcd_SetCursorPosition(5, 1);
    Lcd_WriteCharacter('b');
    
    Lcd_SetCursorPosition(1, 2);
    Lcd_WriteCharacter('c');
    Lcd_SetCursorPosition(5, 2);
    Lcd_WriteCharacter('d');
    
    Lcd_SetCursorPosition(1, 3);
    Lcd_WriteCharacter('e');
    Lcd_SetCursorPosition(5, 3);
    Lcd_WriteCharacter('f');
    
    Lcd_SetCursorPosition(1, 4);
    Lcd_WriteCharacter('g');
    Lcd_SetCursorPosition(5, 4);
    Lcd_WriteCharacter('h');
    
    Lcd_SetCursorPosition(6, 4);
    Lcd_WriteCharacter('i');
    
    Lcd_SetCursorPosition(1, 1);
    Lcd_WriteString("Hello World. Let us see if this works.\0");
    
    for (;;);
}