#ifndef _ERT_TYPEDEF_H
#define _ERT_TYPEDEF_H

/*
 ********************************
            数据类型
 ********************************
 */

/* easyRTOS 数据类型重定义*/
typedef signed char                     ert_int8_t;
typedef signed short                    ert_int16_t;
typedef signed long                     ert_int32_t;
typedef unsigned char                   ert_uint8_t;
typedef unsigned short                  ert_uint16_t;
typedef unsigned long                   ert_uint32_t;
typedef ert_int8_t                      ert_bool_t;

typedef ert_int32_t                     ert_err_t;
typedef ert_uint32_t                    ert_time_t;
typedef ert_uint32_t                    ert_tick_t;
typedef ert_int32_t                     ert_flag_t;
typedef ert_uint32_t                    ert_size_t;
typedef ert_uint32_t                    ert_dev_t;
typedef ert_int32_t                     ert_off_t;


/*  链表节点数据类型  */
struct ert_list_node
{
    /* data */
    struct ert_list_node *next;
    struct ert_list_node *prev;
};
typedef struct ert_list_node ert_list_t;

/*
 ********************************
            对象类型
 ********************************
 */
enum ert_object_class_type
{
    ERT_Object_Class_Thread = 0,        /*线程对象*/
    ERT_Object_Class_Semaphore,         /*信号对象*/
    ERT_Object_Class_Mutex,             /*互斥量对象*/
    ERT_Object_Class_Event,             /*事件对象*/
    ERT_Object_Class_MailBox,           /*邮箱对象*/
    ERT_Object_Class_MessageQueue,      /*消息队列对象*/
    ERT_Object_Class_MemHeap,           /*内存堆对象*/
    ERT_Object_Class_MemPool,           /*内存池对象*/
    ERT_Object_Class_Device,            /*设备对象*/
    ERT_Object_Class_Timer,             /*定时器对象*/
    ERT_Object_Class_Module,            /*模块对象*/
    ERT_Object_Class_Unknown,           /*未知对象*/
    ERT_Object_Class_Static = 0x80      /*静态对象*/
};

#define ERT_NAME_MAX                    8

struct ert_object
{
    /* data */
    char  name[ERT_NAME_MAX];           /*内核对象名字*/
    ert_uint8_t type;                   /*内核对象类型*/
    ert_uint8_t flag;                   /*内核对象状态*/
    ert_uint8_t list;                   /*内核对象的列表节点*/
};
typedef struct ert_object  *ert_object_t;       /*内核对象数据类型重定义*/

struct ert_object_information
{
    /* data */
    enum ert_object_class_type type;    /*对象类型*/
    ert_list_t  object_list;            /*对象列表节点头*/
    ert_size_t  object_size;            /*对象大小*/
};



#define ERT_TRUE                        1
#define ERT_FALSE                       0

/* 错误码重定义 */
#define ERT_EOK                         0   /*no error*/
#define ERT_ERROR                       1   /*a generic error happen*/
#define ERT_ETIMEOUT                    2   /*time out*/
#define ERT_EFULL                       3   /*the resource is full*/
#define ERT_EEMPTY                      4   /*the resource is empty*/
#define ERT_ENOMEM                      5   /*no memort*/
#define ERT_ENOSYS                      6   /*no system*/
#define ERT_EBUSY                       7   /*busy*/
#define ERT_EIO                         8   /*io error*/
#define ERT_EINTR                       9   /*interrupted system call*/
#define ERT_EINVAL                      10  /*invalid argument*/


#define ert_inline                      static __inline
#define ALIGN(n)                        __attribute__((aligned(n)))

#define ERT_ALIGN(size, align)          (((size)+(align)-1) & ~((align)-1))
#define ERT_ALIGN_DOWN(size, align)     ((size)& ~((align)-1))

#define ERT_NULL                        (0)

#endif // !_ERT_TYPEDEF_H