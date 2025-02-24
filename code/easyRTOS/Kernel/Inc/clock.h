#ifndef _CLOCK_H
#define _CLOCK_H

#include "ert_typedef.h"
#include "ert_scheduler.h"

extern ert_tick_t ert_tick;   /*系统时基计数器*/

void ert_tick_increase(void);


#endif // !_CLOCK_H