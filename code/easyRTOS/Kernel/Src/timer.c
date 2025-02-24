#include "timer.h"
#include  "clock.h"
#include  "ert_typedef.h"
#include  "ert_thread.h"


void ert_timer_start(ert_timer_t timer,ert_tick_t time)
{
    timer->timer_status=ERT_TIMER_ACTIVATE;

    timer->start_time=ert_tick;
    timer->current_time=ert_tick;
    timer->target_time=time;
    
}

void ert_timer_stop(ert_timer_t timer)
{
    timer->timer_status=ERT_TIMER_STOP;

    timer->start_time=0;
    timer->current_time=0;
    timer->target_time=0;
    
}
