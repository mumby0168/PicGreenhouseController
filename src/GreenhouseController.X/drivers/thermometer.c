#include <xc.h>
#include "thermometer.h"
#include "../libs/BinaryUtillities.h"
#include "../libs/Delays.h"
#include "lcd.h"

#define TMR2_VAL 11
#define TMR2_TRIGGER_COUNT 12

static void thermometer_write_byte(uchar val)
{
    uchar byBitValue;
    for(uchar i = 0; i < 8; i++)
    {
        byBitValue = (val >> i) & 1;                             //shift the lowest bit                   
        
        RA0 = 0;
        TRISA0 = 0;
        
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                                                              
        NOP();                                     //pull high to low,produce write time    
        
        if(byBitValue == 1)
            TRISA0 = 1; //if write 1,pull high   
        
        DELAY_MICRO_SECONDS(63); //63us
        TRISA0 = 1;                                                                          
        
        NOP();                                                                              
        NOP();                                     //right shift a bit                      
     }
    
    TRISA0 = 1;
}

static uchar thermometer_read_byte(void)
{ 
    uchar ret = 0;    
    for (char i = 0; i < 8; i++)
    {
        RA0 = 0;
        TRISA0 = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();                                    //6us              
        TRISA0 = 1;      
        NOP();                                                       
        NOP();                                                       
        NOP();                                                       
        NOP();                                                       
        NOP();                                   //4us               
        ret |= RA0 << i;                                        
        DELAY_MICRO_SECONDS(63);                             //63us 
    }
       
    return ret;
}

static uchar thermometer_reset(void)
{
    RA0 = 0;
    TRISA0 = 0;          

    DELAY_MICRO_SECONDS(504);

    TRISA0 = 1;  

    uchar uiElapsed = 0;
    while (RA0 == 1)
    {
        if (uiElapsed > 120)
            return 0x01;

        uiElapsed += 2;
    }             

    uiElapsed = 0;
    while (RA0 == 0)
    {
        if (uiElapsed > 240)
            return 0x02;

        uiElapsed += 2;
    };

    DELAY_MICRO_SECONDS(430);
    
    thermometer_write_byte(0xCC); //skip rom
    
    return 0;
}

void Thermometer_Initialise()
{
    g_ProcessTemperatureComplete = false;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.TMR2IF = 0; //clear the flag
    PIE1bits.TMR2IE = 1; //enable timer 2  
    /*
        bit 7 Unimplemented: Read as 0
        bit 6-3 TOUTPS3:TOUTPS0: Timer2 Output Postscale Select bits
        0000 = 1:1 postscale
        0001 = 1:2 postscale
        0010 = 1:3 postscale
        ?
        ?
        ?
        1111 = 1:16 postscale
        bit 2 TMR2ON: Timer2 On bit
        1 = Timer2 is on
        0 = Timer2 is off
        bit 1-0 T2CKPS1:T2CKPS0: Timer2 Clock Prescale Select bits
        00 = Prescaler is 1
        01 = Prescaler is 4
        1x = Prescaler is 16
        ----------------------------------------------
        fclk	pre	post	count	fout	      tout = (4*pre*(PR2-TRM2)*POST*count)/fclk
        4000000	16	16	    12	    1.334101776	  0.749568
        PR2	TRM2	`			
        255	11
     */
    T2CON = 0b01111011; //configure timer 2 as above
    PR2 = 0xFF; //set the trigger value.
    TMR2 = TMR2_VAL;
}

static uchar s_ubyInterruptTriggerCount = 0;
void __interrupt() thermometer_interrupt(void)
{
    if (PIR1bits.TMR2IF == 1)
    {
        s_ubyInterruptTriggerCount++;
        if (s_ubyInterruptTriggerCount > TMR2_TRIGGER_COUNT)
        {
            T2CONbits.TMR2ON = 0; //turn off the timer until the next time process temp is called...
            Lcd_WriteCharacter('1');
            g_ProcessTemperatureComplete = true;
            s_ubyInterruptTriggerCount = 0;
        }
        
        TMR2 = TMR2_VAL;
        PIR1bits.TMR2IF = 0;
    }
}

uchar Themometer_WriteScratchPad(const Thermometer_UserConfig userConfig)
{
    uchar byStatus = 0;
    if ((byStatus = thermometer_reset()) != 0)
        return byStatus;
    
    thermometer_write_byte(0x4E);
    
    for (uchar* pUcEntry = &userConfig; pUcEntry < pUcEntry + sizeof(Thermometer_UserConfig); pUcEntry++)
        thermometer_write_byte(*pUcEntry);
    
    return byStatus;
}

uchar Thermometer_ReadScratchPad(Thermometer_ScratchPad* pScratchPad, uchar byBytesToRead)
{
    uchar byStatus = 0;
    if ((byStatus = thermometer_reset()) != 0)
        return byStatus;

    thermometer_write_byte(0xBE);
    
    if (byBytesToRead > sizeof(Thermometer_ScratchPad))
        byBytesToRead = sizeof(Thermometer_ScratchPad);
    
    for (uchar* pSpEntry = pScratchPad; pSpEntry < pSpEntry + byBytesToRead; pSpEntry++)
        *pSpEntry = thermometer_read_byte();
    
    byStatus = thermometer_reset();
    
    return byStatus;
}

uchar Thermometer_CopyScratchPadToE2(void)
{
    uchar byStatus = 0;
    if ((byStatus = thermometer_reset()) != 0)
        return byStatus;
    
    thermometer_write_byte(0x48);
    
    return byStatus;
}

uchar Thermometer_CopyE2ToScratchPad(void)
{
    uchar byStatus = 0;
    if ((byStatus = thermometer_reset()) != 0)
        return byStatus;
    
    thermometer_write_byte(0xB8);
    
    return byStatus;
}

uchar Thermometer_ProcessTemperature(void)
{
    uchar byStatus = 0;
    if ((byStatus = thermometer_reset()) != 0)
        return byStatus;
    
    thermometer_write_byte(0x44);

    g_ProcessTemperatureComplete = false;
    T2CONbits.TMR2ON = 1;
    return 0;
}

float Thermometer_ConvertTempratureToFloat(uchar ubyTempMsb, uchar ubyTempLsb)
{
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    uchar byIntegerPart = (ubyTempMsb & 0x07) << 4 | GetHighNibble(&ubyTempLsb);
    //Get the low nibble, least significant bit is 2^-4 so divide by 2^4 = 16
    float fDecimalPart = (float)GetLowNibble(&ubyTempLsb) / 16;
    uchar ubySignedPart = ubyTempMsb & 0xF8;
    if (ubySignedPart != 0)
        return (float)byIntegerPart + fDecimalPart - 128;
    
    return (float)byIntegerPart + fDecimalPart;
}

void Thermometer_ConvertTempratureToBcd(uchar ubyTempMsb, uchar ubyTempLsb, Thermometer_BcdTemperature* pBcdTemperature)
{
    pBcdTemperature->bIsNegative = false;
    //get bottom 3 bits of msb and tack it on to the high nibble of lsb to make the integer part
    char byIntegerPart = (ubyTempMsb & 0b00000111) << 4 | GetHighNibble(&ubyTempLsb);
    uchar ubySignedPart = ubyTempMsb & 0xF8;
    short sDecimalValue = 0;

    uchar byDecimalPart = GetLowNibble(&ubyTempLsb);

    //2^-4
    if (byDecimalPart & 0x01)
        sDecimalValue += 625;

    //2^-3
    if (byDecimalPart & 0x02)
        sDecimalValue += 1250;

    //2^-2
    if (byDecimalPart & 0x04)
        sDecimalValue += 2500;

    //2^-1
    if (byDecimalPart & 0x08)
        sDecimalValue += 5000;

    if (ubySignedPart != 0)
    {
        pBcdTemperature->bIsNegative = true;
        byIntegerPart -= 128;
        byIntegerPart *= -1;

        if (byDecimalPart != 0)
        {
            sDecimalValue -= 10000;
            sDecimalValue *= -1;
            byIntegerPart--;
        }
    }

    pBcdTemperature->ubyHundreds = (byIntegerPart / 100);
    pBcdTemperature->ubyTens = ((byIntegerPart / 10) % 10);
    pBcdTemperature->ubyUnits = (byIntegerPart % 10);
    pBcdTemperature->ubyTenths = ((sDecimalValue / 1000));
    pBcdTemperature->ubyHundredths = (((sDecimalValue % 1000) / 100));
    pBcdTemperature->ubyThousandths = (((sDecimalValue % 100) / 10));
    pBcdTemperature->ubyTenThousandths = ((sDecimalValue % 10));
}