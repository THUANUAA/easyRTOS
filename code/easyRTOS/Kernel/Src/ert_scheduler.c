#include "ert_scheduler.h"

/* 线程控制块指针，用于指向当前线程 */
struct ert_thread *ert_current_thread;
/* 线程休眠列表 */
ert_list_t ert_thread_defunct;

/* 定义线程控制块 */ 
extern struct ert_thread ert_flag1_thread;
extern struct ert_thread ert_flag2_thread;
extern struct ert_thread ert_flag3_thread;

/*空闲线程的线程控制块*/
extern struct ert_thread idle;

/* 线程就绪列表 */
ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];
/*
* brief: 初始化系统调度器
*/
void ert_system_scheduler_init(void)
{
    register ert_int32_t offset;

    /* 线程就绪列表初始化 */
    for(offset=0;offset<ERT_THREAD_PRIORITY_MAX;offset++){
        ert_list_init(&ert_thread_priority_table[offset]);
    }

    /* 初始化当前线程控制块指针 */
    ert_current_thread = ERT_NULL;

    /* 初始化线程休眠列表，当线程创建好没有启动之前会被放入到这个列表 */
	ert_list_init(&ert_thread_defunct);
}

void ert_system_scheduler_start(void)
{
    register struct ert_thread *to_thread;

    /* 手动指定第一个运行的线程 */
    to_thread = ert_list_entry(ert_thread_priority_table[0].next,
                                struct ert_thread,
                                tlist);
    ert_current_thread = to_thread;

    /* 切换到第一个线程，该函数在context_rvds.s中实现，在erthw.h 声明
    用于实现第一次线程切换。当一个汇编函数在C文件中调用的时候，
    如果有形参，则执行的时候会将形参传入到CPU寄存器R0中.*/
    ert_hw_context_switch_to((ert_uint32_t)&to_thread->sp);
}

void ert_schedule(void)
{
    struct ert_thread *to_thread;
    struct ert_thread *from_thread;

    ert_int8_t is_idle_flag=1;
    
    for(ert_int32_t i=0;i<ERT_THREAD_PRIORITY_MAX;i++)
    {
        struct ert_thread *thread=ert_list_entry(ert_thread_priority_table[i].next,
                                                struct ert_thread,
                                                tlist);
        if((thread->remaining_tick==0) && (thread->flag==ERT_Object_Class_Thread))
        {
            from_thread = ert_current_thread;
            to_thread = thread;
            ert_current_thread = to_thread;
            is_idle_flag=0;
            break;
        }
    }
    if(ert_current_thread==&idle)return;
    if(is_idle_flag)
    {
        from_thread = ert_current_thread;
        to_thread = &idle;
        ert_current_thread = to_thread;
    }

    /*上下文切换*/
    ert_hw_context_switch((ert_uint32_t)&from_thread->sp,(ert_uint32_t)&to_thread->sp);
}
