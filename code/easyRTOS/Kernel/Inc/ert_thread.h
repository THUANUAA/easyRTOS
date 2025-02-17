#ifndef _ERT_THREAD_H
#define _ERT_THREAD_H

#include "ert_typedef.h"

/*  链表节点数据类型  */
struct ert_list_node
{
    /* data */
    struct ert_list_node *next;
    struct ert_list_node *prev;
};
typedef struct ert_list_node ert_list_t;

/*    线程控制块      */
struct ert_thread
{
    /* data */
    void* sp;                   /*线程栈指针*/
    void* entry;                /*线程入口地址*/
    void* parameter;            /*线程形参*/
    void* stack_addr;           /*线程栈起始地址*/
    ert_uint32_t stack_size;    /*线程栈大小，单位为字节*/
    ert_list_t tlist;
};

typedef struct ert_thread  *ert_thread_t;

ert_bool_t ert_thread_init(struct ert_thread *thread,
    void  (*entry)(void *parameter),
    void  *parameter,
    void  *stack_start,
    ert_uint32_t stack_size);
void ert_list_init(ert_list_t *list);
void ert_list_insert_after(ert_list_t *list,ert_list_t *newNode);
void ert_list_insert_before(ert_list_t *list,ert_list_t *newNode);
void ert_list_delete(ert_list_t *node);

#endif // !_ERT_THREAD_H
