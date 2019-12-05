#ifndef DELAYS_H
#define	DELAYS_H

#include "std.h"

#define DELAY_MICRO_SECONDS(delay) DelayMicroSeconds(2, ((delay - 35) / 3) + 1)

void DelayMicroSeconds(uchar x, uchar y);
void DelayMilliSeconds(unsigned int uiDelay);
void DelaySeconds(unsigned int uiDelay);

#endif

