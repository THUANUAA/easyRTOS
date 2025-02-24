#ifndef _ERT_CONFIG_H
#define _ERT_CONFIG_H

#define ERT_ALIGN_SIZE                      4 /*4字节对齐*/
#define ERT_THREAD_PRIORITY_MAX             32/*优先级数量*/
#define ERT_IDLE_THREAD_PRIORITY_MAX        1/*空闲线程优先级数量*/
#define ERT_TICK_PRE_SECOND                 100/*时钟预分频系数*/
#define ERT_THREAD_SUSPEND_NUM              32/*线程挂起数量*/
#endif // !_ERT_CONFIG_H
