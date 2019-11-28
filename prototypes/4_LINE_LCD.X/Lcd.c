#include <xc.h>
#include "Lcd.h"

#define Enable RA3
#define ReadWrite RA4
#define Reset RA0
#define USE_DATA_REG RA5 = 1;
#define USE_INSTRUCTION_REG RA5 = 0;
#define LCD_BUS_MODE RA2
#define LCD_BUS_MODE_PARALLEL 1

static void lcd_delay()
{
    for (int i=0; i<5000; i++);
}

static void lcd_write_command(const uchar data)
{    
    USE_INSTRUCTION_REG
    TRISD = 0x00;
    Reset = 0;
    ReadWrite = 0; 
    PORTD = data;   
    Enable = 0;
    lcd_delay();
    Enable = 1;
    PORTD = 0;
}

inline static void lcd_select_basic_functionality()
{
    lcd_write_command(0b00110000);
}

inline static void lcd_set_ddram_address(const uchar pos)
{
    lcd_write_command(0b10000000 | pos);
}

static uchar lcd_read_ddram_value(void)
{
    TRISD = 0xFF;
    
    USE_DATA_REG
            
    Reset = 1;
    ReadWrite = 1;
    Enable = 1;
    
    char data = PORTD;
    
    Enable = 0;
    lcd_delay();
    
    Enable = 1;
    
    return data;
}


void Lcd_Init(void)
{
    ADCON1 = 0b00000110; // set adcon 1 for digital a-d ports    
    
    //clear ports
    PORTA = 0x00;
    PORTD = 0x00;
    
    //set a & d to output
    TRISA = 0x00;
    TRISD = 0x00;
    
    LCD_BUS_MODE = LCD_BUS_MODE_PARALLEL;
    
    lcd_select_basic_functionality();
    Lcd_ClearDisplay();
}

inline void Lcd_SetDisplayMode(const bool bLcdOn, const bool bCursorOn, const bool bBlinkOn)
{
    lcd_write_command(0b00001000 | bLcdOn << 2 | bCursorOn << 1 | bBlinkOn);
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
    
    ubyPos -= 1;
    uchar ubyDdramAddress = ubyPos / 2;
    
    switch (ubyLine)
    {
        case 1:
            break;
            
        case 2:
            ubyDdramAddress += 16;
            break;
            
        case 3:
            ubyDdramAddress += 8;
            break;
            
        case 4:
            ubyDdramAddress += 24;
            break;
    }
    
    lcd_set_ddram_address(ubyDdramAddress);
    
    if (ubyPos % 2)
    {
        uchar c = lcd_read_ddram_value();
        lcd_set_ddram_address(0); //Seem to have to reset the address to a different one before actually moving back?
        lcd_set_ddram_address(ubyDdramAddress);
        Lcd_WriteCharacter(c);
    }
}

void Lcd_WriteCharacter(const char c)
{    
    USE_DATA_REG
    Reset = 1;
    ReadWrite = 0;
    PORTD = c;
    Enable = 0;
    lcd_delay();
    Enable = 1;
}

void Lcd_WriteString(const char* pStr)
{
    unsigned char ubyCounter = 0;
    for (const char* p = pStr; *p != '\0'; p++)
    {
        if (ubyCounter % 16 == 0)
        {
            Lcd_SetCursorPosition(1, (ubyCounter / 16) + 1);
            if (*p == ' ')
                continue;
        }
        
        Lcd_WriteCharacter(*p);
        ubyCounter++;
    }
}

inline void Lcd_ClearDisplay()
{
    lcd_write_command(0x01);
}
    