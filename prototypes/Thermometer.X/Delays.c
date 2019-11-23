#include <xc.h>
#include "std.h"
#include "Delays.h"
#include "BitUtillities.h"

enum _tmr0PreScalerValues
{
    TMR0_PRE_SCALER_2 = 0,
    TMR0_PRE_SCALER_4 = 1,
    TMR0_PRE_SCALER_8 = 2,
    TMR0_PRE_SCALER_16 = 3,
    TMR0_PRE_SCALER_32 = 4,
    TMR0_PRE_SCALER_64 = 5,
    TMR0_PRE_SCALER_128 = 6,
    TMR0_PRE_SCALER_256 = 7
};

void EnableTimerZeroInterrupt()
{
    SetBitLow(&INTCON, 2); //Clear the TMR0 overflow bit
    SetBitHigh(&INTCON, 5); //Enable TMR0 interrupt    
}

void ConfigureTimerZeroInterrupt(TimerZeroPreScalerValues byPreScaler)
{
    SetBitLow(&OPTION_REG, 5); //use the internal clock
    SetBitLow(&OPTION_REG, 3); //assign prescaler to timer zero not watch dog timer
    SetLowNibble(&OPTION_REG, (uchar)byPreScaler); // values are defined in enum, as 7 is make bit 4 will always be low which is correct
}

inline void DisableTimerZeroInterrupt()
{
    SetBitLow(&INTCON, 5); //Disable TMR0 interrupt
}

void DelayMicroSeconds(unsigned int uiDelay)
{
    uchar byNumberOfCycles = 1 + (uchar)((double)uiDelay / 512);
    TMR0 = 256 - (uchar)((4000000 * uiDelay) / 4 * byNumberOfCycles);

    ConfigureTimerZeroInterrupt(TMR0_PRE_SCALER_2);
    EnableTimerZeroInterrupt();
    
    uchar byCycleCount = 0;
    while (byCycleCount < byNumberOfCycles)
    {
        while (!IsBitSet(&INTCON, 2));
        SetBitLow(&INTCON, 2);
        byCycleCount++;
    };
    
    DisableTimerZeroInterrupt();
}

void DelayMilliSeconds(unsigned int uiDelay)
{
    uchar byNumberOfCycles = 1 + (uchar)((double)uiDelay / 65.536);
    TMR0 = 256 - ((4000000 * uiDelay) / 4 * 256 * byNumberOfCycles);

    ConfigureTimerZeroInterrupt(TMR0_PRE_SCALER_256);
    EnableTimerZeroInterrupt();
    
    uchar byCycleCount = 0;
    while (byCycleCount < byNumberOfCycles)
    {
        while (!IsBitSet(&INTCON, 2));
        SetBitLow(&INTCON, 2);
        byCycleCount++;
    };
    
    DisableTimerZeroInterrupt();
}

void DelaySeconds(unsigned int uiDelay)
{
    uchar byNumberOfCycles = 1 + (uchar)((double)uiDelay / 0.065536);
    TMR0 = 256 - ((4000000 * uiDelay) / 4 * 256 * byNumberOfCycles);

    ConfigureTimerZeroInterrupt(TMR0_PRE_SCALER_256);
    EnableTimerZeroInterrupt();
    
    uchar byCycleCount = 0;
    while (byCycleCount < byNumberOfCycles)
    {
        while (!IsBitSet(&INTCON, 2));
        SetBitLow(&INTCON, 2);
        byCycleCount++;
    };
    
    DisableTimerZeroInterrupt();
}