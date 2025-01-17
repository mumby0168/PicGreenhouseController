#include "lcd.h"

#define RS RA1
#define RW RA2
#define Enable RA3

inline void WriteCommand(uchar cmd)
{
    TRISD = 0x00;
    RS = 0;
    RW = 0; 
    PORTD = cmd;   
    Enable = 0;
    delay();
    Enable = 1;
    PORTD = 0;
}

void delay() { int i;for(i=0;i<5000;i++); }

void SetDisplayMode(bool displayOn, bool cursorOn, bool cursorBlink)
{
    WriteCommand(0b00001000 | displayOn << 2 | cursorOn << 1 | cursorBlink);  
}

//If display two lines is true, display large font will be ignored
void SetDisplayResolution(bool displayTwoLines, bool displayLargeFont)
{
    WriteCommand(0b00110000 | displayTwoLines << 3 | displayLargeFont << 2);
}

void ClearDisplay()
{
    WriteCommand(0x1);
}

void ShiftCursor(bool right)
{
    WriteCommand(0b00010000 | right << 2);
}

void ShiftDisplay(bool right)
{
    WriteCommand(0b00011000 | right << 2);
}

void MoveCursorToStart()
{
    WriteCommand(0x02);
}

void SetDdramAddress(uchar addr)
{
    WriteCommand(0b10000000 | addr);
}

void SetCursorPosition(bool secondLine, uchar pos)
{
    MoveCursorToStart();
    if (pos > 39)
        pos = 39;
    
    if (secondLine)
        pos += 40;
    
    SetDdramAddress(pos);
}

void WriteCharacter(char c)
{
    RS = 1;
    RW = 0;
    PORTD = c;
    Enable = 0;
    delay();
    Enable = 1;
}

void WriteNumber(char num)
{
    if(num > 100)
    {
        WriteCharacter(126);
        return;
    }
    if(num > 10)
    {
        char tens = num / 10;
        WriteCharacter(tens + 48);
    }
    char digits = num % 10;
    WriteCharacter(digits + 48);
}

void Initialise()
{
    // 1: input 0: output
    ADCON1 = 0x07;
    TRISA = 0b00000000;
    TRISD = 0b00000000;
    PORTA = 0x00;
    PORTD = 0x00;
}
