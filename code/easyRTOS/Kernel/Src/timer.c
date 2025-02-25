#include "timer.h"
#include  "clock.h"
#include  "ert_typedef.h"
#include  "ert_thread.h"

void system_timer_init(ert_timer_t timer,
                        void  (*timeout_func)(void *parameter),
                        void  *parameter,
                        ert_bool_t timer_status)
{
    ert_timer_init(timer,timeout_func,parameter,timer_status);
    
}

void ert_timer_init(ert_timer_t timer,
                    void  (*timeout_func)(void *parameter),
                    void  *parameter,
                    ert_bool_t timer_status)
{

    timer->timeout_func=timeout_func;
    timer->parameter=parameter;
    timer->timer_status=timer_status;
}

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
