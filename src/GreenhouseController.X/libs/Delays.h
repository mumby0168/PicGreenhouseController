#ifndef DELAYS_H
#define	DELAYS_H

typedef enum _tmr0PreScalerValues TimerZeroPreScalerValues;


#define DELAY_MICRO_SECONDS(delay) \
\
{ \
    const char y = ((delay - 35) / 3) + 1; \
    char x = 2, z = 0; \
    do \
    { \
        z = y; \
        do {;} while(--z); \
    } while(--x); \
} \

void DelayMilliSeconds(unsigned int uiDelay);
void DelaySeconds(unsigned int uiDelay);


#endif

