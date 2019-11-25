#include <xc.h>
#include "std.h"
#include "BitUtillities.h"
#include "Delays.h"
#include "example.h"
void WriteByte(uchar byte)
{
    for (char i = 0; i < 8; i++)
    {                 
    TRISA0 = 0;
    RA0 = 0;//shift the lowest bit                                                                             
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                     //pull high to low,produce write time    
   if ((byte >> i) & 1)  TRISA0 = 1;                    //if write 1,pull high                   
   delay(2,7);                                //delay 63us                             
   TRISA0 = 1;                                                                          
   NOP();                                                                              
   NOP();                                                                                                              //right shift a bit
    }
    
    TRISA0 = 1;
}

uchar ReadByte(void)
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
        delay(2,7);                              //63us 
    }
       
    return ret;
}

uchar Reset(void)
{
    RA0 = 0;
    TRISA0 = 0;          

    delay(2,70);

    TRISA0 = 1;  

    uchar uiElapsed = 0;
    while (RA0 == 1)
    {
        if (uiElapsed > 120)
            return 0x12;

        uiElapsed += 2;
    }             

    uiElapsed = 0;
    while (RA0 == 0)
    {
        if (uiElapsed > 240)
            return 0x13;

        uiElapsed += 2;
    };

    delay(2,60);
    
    WriteByte(0xCC); //skip rom
    
    return 0;
}

uchar ConvertT(void)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;
    
    WriteByte(0x44);
    return 0;
}

typedef struct _scratchPad
{
    uchar byTempLsb;
    uchar byTempMsb;
    uchar byUserOne;
    uchar byUserTwo;
    uchar byConfig;
    uchar byRes1;
    uchar byRes2;
    uchar byRes3;
    uchar byCRC;
} ScratchPad;


uchar ReadScratchpad(ScratchPad* pScratchPad)
{
    uchar byStatus = 0;
    if ((byStatus = Reset()) != 0)
        return byStatus;

    WriteByte(0xBE);

    for (uchar* pSpEntry = pScratchPad; pSpEntry < pSpEntry + sizeof(ScratchPad); pSpEntry++)
        *pSpEntry = ReadByte();

    byStatus = Reset();
    
    return byStatus;
}