#include <xc.h> 
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

typedef unsigned char uchar;
typedef uchar bool;
#define true 1
#define false 0


#define Enable RA3
#define ReadWrite RA4
#define Reset RA0
#define USE_DATA_REG RA5 = 1;
#define USE_INSTRUCTION_REG RA5 = 0;
#define LCD_BUS_MODE RA2
#define LCD_BUS_MODE_PARALLEL 1

void delay() { int i;for(i=0;i<5000;i++); }

static void lcd_write_command(const uchar data)
{    
    USE_INSTRUCTION_REG
    TRISD = 0x00;
    Reset = 0;
    ReadWrite = 0; 
    PORTD = data;   
    Enable = 0;
    delay();
    Enable = 1;
    PORTD = 0;
}

void Lcd_WriteCharacter(const char c)
{    
    USE_DATA_REG
    Reset = 1;
    ReadWrite = 0;
    PORTD = c;
    Enable = 0;
    delay();
    Enable = 1;
}

inline void Lcd_SetDdramAddress(const uchar pos)
{
    lcd_write_command(0b10000000 | pos);
}

inline void Lcd_SetDisplayMode(const bool bLcdOn, const bool bCursorOn, const bool bBlinkOn)
{
    lcd_write_command(0b00001000 | bLcdOn << 2 | bCursorOn << 1 | bBlinkOn);
}

void Lcd_Init()
{
    ADCON1 = 0b00000110; // set adcon 1 for digital a-d ports    
    
    //clear ports
    PORTA = 0x00;
    PORTD = 0x00;
    
    //set a & d to output
    TRISA = 0x00;
    TRISD = 0x00;
    
    LCD_BUS_MODE = LCD_BUS_MODE_PARALLEL;
    
    lcd_write_command(0b00110000); //Select 8 bit interface. Basic mode
}

inline void Lcd_SelectBasicFunctionality()
{
    lcd_write_command(0b00110000);
}

inline void Lcd_SelectExtenedFunctionality()
{
    lcd_write_command(0b00110100);
}

uchar Lcd_ReadDdramValue()
{
    TRISD = 0xFF;
    
    USE_DATA_REG
            
    Reset = 1;
    ReadWrite = 1;
    Enable = 1;
    
    char data = PORTD;
    
    Enable = 0;
    delay();
    
    Enable = 1;
    
    return data;
}

void Lcd_SetCursorPosition(uchar ubyPos, uchar ubyLine)
{
    if (ubyPos < 1)
        ubyPos = 1;
    
    if (ubyPos > 16)
        ubyPos = 16;
    
    if (ubyLine < 1)
        ubyLine = 1;
    
    if (ubyLine > 4)
        ubyLine = 16;
    
    uchar ubyDdramAddress = ubyPos / 2;
    
    switch (ubyLine)
    {
        case 1:
            break;
            
        case 2:
            ubyDdramAddress += 16;
            
        case 3:
            ubyDdramAddress += 8;
            
        case 4:
            ubyDdramAddress += 24;
    }
    
    Lcd_SetDdramAddress(ubyDdramAddress);
}

void main(void) 
{    
    Lcd_Init();
    Lcd_SelectBasicFunctionality();
    Lcd_SetDisplayMode(true, false, false); //Turn on display and cursor and blink
//
//    Lcd_SetDdramAddress(2);
//    Lcd_WriteCharacter('a');
//    
//    Lcd_SetDdramAddress(2);
//    char c = Lcd_ReadDdramValue();
//    Lcd_SetDdramAddress(2);
//    
//    Lcd_SetDdramAddress(0);
//    Lcd_WriteCharacter(c);
//    
//    Lcd_SetDdramAddress(0);
//    ///////////////////////////////////////
//    Lcd_WriteCharacter('a');
//    Lcd_WriteCharacter('b');
//    Lcd_WriteCharacter('c');
//    Lcd_WriteCharacter('d');
//    
//    Lcd_SetDdramAddress(0);
//    char c = Lcd_ReadDdramValue();
//    Lcd_ReadDdramValue();
//    Lcd_SetDdramAddress(2);
//    Lcd_SetDdramAddress(0);
//    Lcd_WriteCharacter(c);
//    Lcd_WriteCharacter('2');
    
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
    
    for (;;);
}