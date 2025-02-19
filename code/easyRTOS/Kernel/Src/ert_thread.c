#include "ert_thread.h"
#include "ert_cpuport.h"
#include "ert_object.h"

/* 线程控制块指针，用于指向当前线程 */
extern struct ert_thread *ert_current_thread;

/*
*brief thread 初始化
*param1: 线程控制块指针
*param2: entry 是线程函数名，表示线程的入口
*param3: parameter 是线程形参，用于传递线程参数
*param4: stack_start 用于指向线程栈的起始地址
*param5: stack_size 表示线程栈的大小，单位为字节
*/
ert_bool_t ert_thread_init(struct ert_thread *thread,
                            const char *name,
                            void  (*entry)(void *parameter),
                            void  *parameter,
                            void  *stack_start,
                            ert_uint32_t stack_size)
{
    /*线程对象初始化*/
    ert_object_init((ert_object_t)thread,ERT_Object_Class_Thread,name);

    ert_list_init(&(thread->tlist));

    thread->entry = (void *)entry;
    thread->parameter=parameter;
    thread->stack_addr=stack_start;
    thread->stack_size=stack_size;
    thread->remaining_tick=0;
    /* 初始化线程栈，并返回线程栈指针 */
    thread->sp=(void *)ert_hw_stack_init(
        thread->entry,
        thread->parameter,
        (void *)((char *)thread->stack_addr + thread->stack_size - 4) 
    );
    
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
    struct ert_thread *thread;

    /*获取当前线程的线程控制块*/
    thread=ert_current_thread;

    /*设置延时时间*/
    thread->remaining_tick=tick;

    /*系统调度*/
    ert_schedule();
}
