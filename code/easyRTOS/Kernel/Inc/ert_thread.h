#ifndef _ERT_THREAD_H
#define _ERT_THREAD_H

#include "ert_typedef.h"

/*    线程控制块      */
struct ert_thread
{
    /* data */
    ert_int8_t  name[ERT_NAME_MAX];     /*内核对象名字*/
    ert_uint8_t type;                   /*内核对象类型*/
    ert_uint8_t flag;                   /*内核对象状态*/
    ert_uint8_t list;                   /*内核对象的列表节点*/
    
    ert_list_t tlist;                   /*线程链表节点*/
    void* sp;                           /*线程栈指针*/
    void* entry;                        /*线程入口地址*/
    void* parameter;                    /*线程形参*/
    void* stack_addr;                   /*线程栈起始地址*/
    ert_uint32_t stack_size;            /*线程栈大小，单位为字节*/
    
};

typedef struct ert_thread  *ert_thread_t;

ert_bool_t ert_thread_init(struct ert_thread *thread,
    const char *name,
    void  (*entry)(void *parameter),
    void  *parameter,
    void  *stack_start,
    ert_uint32_t stack_size);
void ert_list_init(ert_list_t *list);
void ert_list_insert_after(ert_list_t *list,ert_list_t *newNode);
void ert_list_insert_before(ert_list_t *list,ert_list_t *newNode);
void ert_list_delete(ert_list_t *node);

ert_int8_t *ert_strncpy(char *dst,const char *src,ert_uint32_t n);

#endif // !_ERT_THREAD_H
