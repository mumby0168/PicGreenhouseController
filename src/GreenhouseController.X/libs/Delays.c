#include <xc.h>
#include "std.h"
#include "Delays.h"
#include "BinaryUtillities.h"

typedef enum _tmr0PreScalerValues
{
    TMR0_PRE_SCALER_2 = 0,
    TMR0_PRE_SCALER_4 = 1,
    TMR0_PRE_SCALER_8 = 2,
    TMR0_PRE_SCALER_16 = 3,
    TMR0_PRE_SCALER_32 = 4,
    TMR0_PRE_SCALER_64 = 5,
    TMR0_PRE_SCALER_128 = 6,
    TMR0_PRE_SCALER_256 = 7
} TimerZeroPreScalerValues;

static void enable_timer_zero_interrupt()
{
    INTCONbits.GIE = 0;
    SetBitLow(&INTCON, 2); //Clear the TMR0 overflow bit
    SetBitHigh(&INTCON, 5); //Enable TMR0 interrupt    
}

static void configure_timer_zero_interrupt(TimerZeroPreScalerValues byPreScaler)
{
    SetBitLow(&OPTION_REG, 5); //use the internal clock
    SetBitLow(&OPTION_REG, 3); //assign prescaler to timer zero not watch dog timer
    SetLowNibble(&OPTION_REG, (uchar)byPreScaler); // values are defined in enum, as 7 is make bit 4 will always be low which is correct
}

inline static void disable_timer_zero_interrupt()
{
    SetBitLow(&INTCON, 5); //Disable TMR0 interrupt
    INTCONbits.GIE = 1;
}

void DelayMicroSeconds(uchar x, uchar y)
{
    uchar z = 0;
    do
    {
        z = y;
        do {;} while(--z);
    } while(--x);
}

void DelayMilliSeconds(unsigned int uiDelay)
{
    uchar byNumberOfCycles = 1 + (uchar)((double)uiDelay / 65.536);
    TMR0 = 256 - ((4000000 * uiDelay) / 4 * 256 * byNumberOfCycles);

    configure_timer_zero_interrupt(TMR0_PRE_SCALER_256);
    enable_timer_zero_interrupt();
    
    uchar byCycleCount = 0;
    while (byCycleCount < byNumberOfCycles)
    {
        while (!IsBitSet(&INTCON, 2));
        SetBitLow(&INTCON, 2);
        byCycleCount++;
    };
    
    disable_timer_zero_interrupt();
}

void DelaySeconds(unsigned int uiDelay)
{
    uchar byNumberOfCycles = 1 + (uchar)((double)uiDelay / 0.065536);
    TMR0 = 256 - ((4000000 * uiDelay) / 4 * 256 * byNumberOfCycles);

    configure_timer_zero_interrupt(TMR0_PRE_SCALER_256);
    enable_timer_zero_interrupt();
    
    uchar byCycleCount = 0;
    while (byCycleCount < byNumberOfCycles)
    {
        while (!IsBitSet(&INTCON, 2));
        SetBitLow(&INTCON, 2);
        byCycleCount++;
    };
    
    disable_timer_zero_interrupt();
}