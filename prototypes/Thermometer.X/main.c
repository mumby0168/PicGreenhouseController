#include <xc.h>
#include "std.h"
#include "BitUtillities.h"
#include "Delays.h"
#include "Lcd.h"
#include "Thermometer.h"

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

void main(void) 
{
    Lcd_Init();    
    
    Lcd_WriteString("Start.");
    DelaySeconds(3);
    while (true)
    {          
        Lcd_ClearDisplay();
        
        uchar byRes = Thermometer_ProcessTemperature();
                
        Thermometer_ScratchPad sp;
        byRes = Thermometer_ReadScratchPad(&sp, sizeof(Thermometer_ScratchPad));
        
        Thermometer_BcdTemperature temperatureBcdValue;
        Thermometer_ConvertTempratureToBcd(0b00000000, 0b00001000, &temperatureBcdValue);
        
        if (byRes == 0)
        {
            Lcd_WriteString("Success: ");
            Lcd_SetCursorPosition(10, 1);
            if (temperatureBcdValue.bIsNegative)
                Lcd_WriteCharacter('-');
            
            Lcd_WriteCharacter(temperatureBcdValue.ubyHundreds + 48);
            Lcd_WriteCharacter(temperatureBcdValue.ubyTens + 48);
            Lcd_WriteCharacter(temperatureBcdValue.ubyUnits + 48);
            Lcd_WriteCharacter('.');
            Lcd_WriteCharacter(temperatureBcdValue.ubyTenths + 48);
            Lcd_WriteCharacter(temperatureBcdValue.ubyHundredths + 48);
            Lcd_WriteCharacter(temperatureBcdValue.ubyThousandths + 48);
            Lcd_WriteCharacter(temperatureBcdValue.ubyTenThousandths + 48);
        }
        else
        {
            Lcd_WriteString("Failed: ");
            Lcd_WriteCharacter(byRes + 48);
        }
        
        DelaySeconds(1);
    };
    
    return;
}
