#include "clock.h"
#include "ethw.h"
#include "ert_thread.h"

static ert_tick_t ert_tick=0;   /*系统时基计数器*/
extern ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];

void ert_tick_increase(void)
{
    ert_uint8_t i;
    
    ert_tick++;

    /*扫描就绪列表中所有线程的remaining_tick,如果不为0，则减1*/
    for(i=0;i<thread_num;i++)
    {
        struct ert_list_node *node = ert_thread_priority_table[i].next;
        
        // 遍历线程链表
        while (node != &ert_thread_priority_table[i])
        {
            struct ert_thread *thread = ert_list_entry(node, struct ert_thread, tlist);
            node = node->next;

            if(thread->remaining_tick>0)
            {
                thread->remaining_tick--;
            }
        }
        
    }
    /*系统调度*/
    ert_schedule();
}
