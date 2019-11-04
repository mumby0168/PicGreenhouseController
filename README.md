# PIC Greenhouse Controller
This is an embedded systems project conducted at the University of Hull the task set out is to create a greenhouse controller using the QL200 development which runs a PIC16F877A.

## System Specification
Create a Green House Control system using the PIC QL200 development board. This system will contain a Time of Day clock and provide the following features and displays:    
* Setting of current time, date and week day  
* Display of current temperature  
* Display of current time, date and week day 
* Entry of trigger temperatures (temperature thresholds)  
* Two output controls, one for a heating circuit and one for air circulating:  
  - When the temperature drops below the set low temperature threshold, the heating control output will be turned on. If the temperature continues to drop an alarm signal is produced. The user should be able to clear an alarm by pressing a button on the device.  
  - When the temperature rises above the set high temperature threshold the heating control output will be turned off and the air venting control should be turned on to cool the temperature down. If the temperature continuses to rise and alarm signal is produced. The user should be able to clear the alarm by pressing a button. 
* There should be a day time and a night time operation modes. The day time should begin at 6:30 am in the morning and end at 7:30 pm; outside this time is night time mode. The system should allow to configure different temperature settings for day time and night time modes. 
 
The output control status can be indicated by LEDs. 
The resolution of temperature only need to be 1 decimal point. 

# Getting Started
In order to get started using the chip the following code is required:
```c
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
```
