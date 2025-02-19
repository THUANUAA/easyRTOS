#ifndef _ERT_THREAD_H
#define _ERT_THREAD_H

#include "ert_typedef.h"
#include "ert_scheduler.h"

/*    线程控制块      */
struct ert_thread
{
    ert_list_t tlist;                   /*线程链表节点*/
    void* sp;                           /*线程栈指针*/
    void* entry;                        /*线程入口地址*/
    void* parameter;                    /*线程形参*/
    void* stack_addr;                   /*线程栈起始地址*/
    ert_uint32_t stack_size;            /*线程栈大小，单位为字节*/
    
    ert_uint32_t remaining_tick;        /*用于实现阻塞延时*/

    ert_uint8_t  thread_priority;
};

typedef struct ert_thread  *ert_thread_t;

extern ert_uint8_t thread_num;

ert_bool_t ert_thread_init(struct ert_thread *thread,
    void  (*entry)(void *parameter),
    void  *parameter,
    void  *stack_start,
    ert_uint32_t stack_size,
    ert_uint8_t  thread_priority);
void ert_list_init(ert_list_t *list);
void ert_list_insert_after(ert_list_t *list,ert_list_t *newNode);
void ert_list_insert_before(ert_list_t *list,ert_list_t *newNode);
void ert_list_delete(ert_list_t *node);

void ert_thread_delay(ert_tick_t tick);

#endif // !_ERT_THREAD_H
