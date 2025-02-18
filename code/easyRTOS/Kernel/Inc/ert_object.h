#ifndef _ERT_OBJECT_H
#define _ERT_OBJECT_H

#include "ert_typedef.h"
#include "ert_thread.h"
#include "ethw.h"

enum ert_object_info_type
{   
    ERT_Object_Info_Thread = 0,         /*对象是线程*/
#ifdef ERT_USING_SEMAPHORE
    ERT_Object_Info_Semaphore,          /*对象是信号量*/
#endif
#ifdef ERT_USING_MUTEX
    ERT_Object_Info_Mutex,              /*对象是互斥量*/
#endif
#ifdef ERT_USING_EVENT
    ERT_Object_Info_Event,              /*对象是事件*/
#endif
#ifdef ERT_USING_MAILBOX
    ERT_Object_Info_MailBox,            /*对象是邮箱*/
#endif
#ifdef ERT_USING_MESSAGEQUEUE
    ERT_Object_Info_MessageQueue,       /*对象是消息队列*/
#endif
#ifdef ERT_USING_MEMHEAP
    ERT_Object_Info_MemHeap,            /*对象是内存堆*/
#endif
#ifdef ERT_USING_MEMPOOL
    ERT_Object_Info_MemPool,            /*对象是内存池*/
#endif
#ifdef ERT_USING_DEVICE
    ERT_Object_Info_Device,             /*对象是设备*/
#endif

    ERT_Object_Info_Timer,              /*对象是定时器*/

#ifdef ERT_USING_MODULE
    ERT_Object_Info_Module,             /*对象是模块*/
#endif
    ERT_Object_Info_Unknown,            /*对象未知*/
};

#define _OBJ_CONTATINER_LIST_INIT(c)\
    {&(ert_object_container[c].object_list),&(ert_object_container[c].object_list)}

void ert_object_init(struct ert_object *object,enum ert_object_class_type type, const char *name);

#endif // !_ERT_OBJECT_H