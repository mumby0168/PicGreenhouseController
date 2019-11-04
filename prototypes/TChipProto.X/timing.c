/*
 * File:   timing.c
 * Author: billy
 *
 * Created on 04 November 2019, 13:40
 */
#include <xc.h> 
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)#
pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config


void main(void) {
    TRISC =  0x00;
    PORTC = 0x55; 
    for (;;);
}
