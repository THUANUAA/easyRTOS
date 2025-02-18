#include "ert_object.h"

static struct ert_object_information ert_object_container[ERT_Object_Info_Unknown]=
{
    /*初始化对象容器 - 线程*/
    {
        ERT_Object_Class_Thread,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Thread),
        sizeof(struct ert_thread)   
    },

#ifdef ERT_USING_SEMAPHORE
    /*初始化对象容器 - 信号量*/
    {
        ERT_Object_Class_Semaphore,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Semaphore),
        sizeof(struct ert_semaphore)   
    },
#endif

#ifdef ERT_USING_MUTEX
    /*初始化对象容器 - 互斥量*/
    {
        ERT_Object_Class_Mutex,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Mutex),
        sizeof(struct ert_mutex)   
    },
#endif

#ifdef ERT_USING_EVENT
    /*初始化对象容器 - 事件*/
    {
        ERT_Object_Class_Event,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Event),
        sizeof(struct ert_event)   
    },
#endif

#ifdef ERT_USING_MAILBOX
    /*初始化对象容器 - 邮箱*/
    {
        ERT_Object_Class_MailBox,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_MailBox),
        sizeof(struct ert_mailbox)   
    },
#endif

#ifdef ERT_USING_MESSAGEQUEUE
    /*初始化对象容器 - 消息队列*/
    {
        ERT_Object_Class_MessageQueue,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_MessageQueue),
        sizeof(struct ert_messagequeue)   
    },
#endif

#ifdef ERT_USING_MEMHEAP
    /*初始化对象容器 - 内存堆*/
    {
        ERT_Object_Class_MemHeap,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_MemHeap),
        sizeof(struct ert_memheap)   
    },
#endif

#ifdef ERT_USING_MEMPOOL
    /*初始化对象容器 - 内存池*/
    {
        ERT_Object_Class_MemPool,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_MemPool),
        sizeof(struct ert_mempool)   
    }
#endif

#ifdef ERT_USING_DEVICE
    /*初始化对象容器 - 设备*/
    {
        ERT_Object_Class_Device,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Device),
        sizeof(struct ert_device)
    },
#endif

    // /*初始化对象容器 - 定时器*/
    // {
    //     ERT_Object_Class_Timer,
    //     _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Timer),
    //     sizeof(struct ert_Timer)   
    // },

#ifdef ERT_USING_MODULE
    /*初始化对象容器 - 模块*/
    {
        ERT_Object_Class_Module,
        _OBJ_CONTATINER_LIST_INIT(ERT_Object_Info_Module),
        sizeof(struct ert_module)   
    }
#endif

};

/*
*brief: 从容器中获取指定类型的对象的信息
*param: type 指定的类型的对象
*return: 返回一个指向对象信息结构体的指针。如果未找到指定类型的对象，返回 ERT_NULL
*/
struct ert_object_information *ert_object_get_information(enum ert_object_class_type type)
{
    int index;
    for(index=0;index < ERT_Object_Info_Unknown;index++)
        if(ert_object_container[index].type == type)
            return &ert_object_container[index];
    
    return ERT_NULL;
}

/*
*brief: 初始化对象并将对象添加到对象容器中
*param1: 初始化的对象
*param2: 对象的类型
*param3: 对象的名字，必须唯一
*/
void ert_object_init(struct ert_object *object,enum ert_object_class_type type, const char *name)
{
    register ert_int32_t temp;
    struct ert_object_information *information;

    /*获取对象信息，即从容器里拿取对应对象列表头指针*/
    information = ert_object_get_information(type);

    /*设置对象类型为静态*/
    object->type = type | ERT_Object_Class_Static;

    /*拷贝名字*/
    ert_strncpy(object->name,name,ERT_NAME_MAX);

    /*关中断*/
    temp = ert_hw_interrupt_disable();

    /*将对象插入到容器的对应列表中,不同类型的对象所在的列表不一样*/
    ert_list_insert_after(&(information->object_list),&(object->list));

    /*使能中断*/
    ert_hw_interrupt_enable(temp);
}
