#include "clock.h"
#include "ethw.h"
#include "ert_thread.h"

static ert_tick_t ert_tick=0;   /*系统时基计数器*/
extern ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];

void ert_tick_increase(void)
{
    ert_uint32_t i;
    struct ert_thread *thread;
    ert_tick++;

    /*扫描就绪列表中所有线程的remaining_tick,如果不为0，则减1*/
    for(i=0;i<ERT_THREAD_PRIORITY_MAX;i++)
    {
        thread=ert_list_entry(ert_thread_priority_table[i].next,struct ert_thread,tlist);
        if(thread->remaining_tick>0)
        {
            thread->remaining_tick--;
        }
    }
    /*系统调度*/
    ert_schedule();
}
