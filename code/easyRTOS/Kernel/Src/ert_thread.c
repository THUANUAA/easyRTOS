#include "ert_thread.h"
#include "ert_cpuport.h"


ert_uint8_t thread_num=0;

/* 线程就绪列表 */
extern ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];
/* 线程挂起列表*/
extern ert_list_t ert_thread_suspend_table[ERT_THREAD_SUSPEND_NUM];

/*
*brief thread 初始化
*param1: 线程控制块指针
*param2: entry 是线程函数名，表示线程的入口
*param3: parameter 是线程形参，用于传递线程参数
*param4: stack_start 用于指向线程栈的起始地址
*param5: stack_size 表示线程栈的大小，单位为字节
*/
ert_bool_t ert_thread_init(struct ert_thread *thread,
                            void  (*entry)(void *parameter),
                            void  *parameter,
                            void  *stack_start,
                            ert_uint32_t stack_size,
                            ert_uint8_t  thread_priority)
{
    ert_list_init(&(thread->tlist));

    thread->entry = (void *)entry;
    thread->parameter=parameter;
    thread->stack_addr=stack_start;
    thread->stack_size=stack_size;
    thread->remaining_tick=0;
    thread->thread_priority=thread_priority;

    /* 初始化线程栈，并返回线程栈指针 */
    thread->sp=(void *)ert_hw_stack_init(
        thread->entry,
        thread->parameter,
        (void *)((char *)thread->stack_addr + thread->stack_size - 4) 
    );
    thread_num++;
    thread->status=ERT_THREAD_ACTIVATE;
    /*将线程插入就绪列表中*/
    ert_list_insert_before(&(ert_thread_priority_table[thread->thread_priority]),&(thread->tlist));
    
    thread->remaining_tick=100*(thread_priority+1);
        
    ert_timer_start(&(thread->thread_timer),50);

    return ERT_EOK;
}

/*
*brief 节点链表初始化
*param: list 链表头节点
*/
void ert_list_init(ert_list_t *list)
{
    list->next=list->prev=list;
}

/*
*brief 双向链表表头后面插入一个节点
*param1: list 链表头节点
*param2: newNode 新插入节点
*/
void ert_list_insert_after(ert_list_t *list,ert_list_t *newNode)
{
    list->next->prev=newNode;
    newNode->next=list->next;

    list->next=newNode;
    newNode->prev=list;
}

/*
*brief 双向链表表头前面插入一个节点
*param1: list 链表头节点
*param2: newNode 新插入节点
*/
void ert_list_insert_before(ert_list_t *list,ert_list_t *newNode)
{
    list->prev->next=newNode;
    newNode->prev=list->prev;

    list->prev=newNode;
    newNode->next=list;
}

/*
*brief 从双向链表删除一个节点 
*param1: node 被删除的节点
*/
void ert_list_delete(ert_list_t *node)
{
    node->next->prev=node->prev;
    node->prev->next=node->next;

    node->next=node->prev=node;
}

void ert_thread_delay(ert_tick_t tick)
{
    ert_thread_t thread;

    /*获取当前线程的线程控制块*/
    thread=ert_current_thread;

    /*设置延时时间*/
    thread->remaining_tick=tick;

    /*系统调度*/
    ert_slice_schedule();
    
}

void ert_thread_suspend(ert_thread_t thread)
{
    thread->status=ERT_THREAD_SUSPEND;
    
    ert_list_delete(&thread->tlist);

    ert_list_insert_before(&(ert_thread_suspend_table[thread->thread_priority]),&(thread->tlist));
}

void ert_thread_activate(ert_thread_t thread)
{
    thread->status=ERT_THREAD_ACTIVATE;
    thread->remaining_tick=100*(thread->thread_priority+1);
    ert_list_delete(&thread->tlist);
    
    ert_list_insert_before(&(ert_thread_priority_table[thread->thread_priority]),&(thread->tlist));
}

void ert_thread_alive(ert_tick_t tick)
{
    /*获取当前线程的线程控制块*/
    ert_thread_t thread=ert_current_thread;

    thread->status=ERT_THREAD_ACTIVATE;
    thread->remaining_tick=tick;
    // thread->thread_timer->start_time=ert_tick;
    // thread->thread_timer->target_time=tick;
    // thread->thread_timer->timer_status=ERT_TIMER_ACTIVATE;
    //ert_timer_start(thread->thread_timer, tick);
}