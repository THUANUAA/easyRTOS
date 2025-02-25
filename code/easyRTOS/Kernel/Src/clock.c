#include "clock.h"
#include "ethw.h"
#include "ert_thread.h"


ert_tick_t ert_tick=0;   /*系统时基计数器*/
extern ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];
extern ert_list_t ert_thread_suspend_table[ERT_THREAD_PRIORITY_MAX];

void ert_tick_increase(void)
{
    ert_thread_t thread=ert_current_thread;

    ert_tick++;

    thread->remaining_tick--;

    if(thread->remaining_tick==0)
    {
        ert_thread_suspend(thread);       
        /*系统调度*/
        ert_slice_schedule();      
    }
    else
    {
        ert_preemptive_schedule();
    }

    if((ert_tick-system_timer.start_time)==system_timer.target_time)
    {
        ert_timer_stop(&system_timer);
        Timer_Handler(ERT_NULL);
        ert_timer_start(&system_timer,ERT_STSTEM_TIMER_TIME);
    }


    if(ert_tick%200==0)
    {
        for(ert_uint8_t i=0;i<thread_num;i++)
        {
            thread = ert_list_entry(ert_thread_suspend_table[i].next,
                                    struct ert_thread, 
                                    tlist);
            if(thread->status==ERT_THREAD_SUSPEND)
                ert_thread_activate(thread);
        }
    }
}
