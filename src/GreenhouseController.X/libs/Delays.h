#ifndef DELAYS_H
#define	DELAYS_H

#include "std.h"

#define USING_DELAY_MICRO_SECONDS static char s_dms_x, s_dms_y, s_dms_z;

#define DELAY_MICRO_SECONDS(delay)\
    s_dms_x = 2;\
    s_dms_y = ((delay - 35) / 3) + 1;\
    s_dms_z = 0;\
    do\
    {\
        s_dms_z = s_dms_y;\
        do {;} while(--s_dms_z);\
    } while(--s_dms_x);\

void DelayMicroSeconds(uchar x, uchar y);
void DelayMilliSeconds(unsigned int uiDelay);
void DelaySeconds(unsigned int uiDelay);

#endif

