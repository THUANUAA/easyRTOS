#include "idle.h"

#define IDLE_THREAD_STACK_SIZE      512

ALIGN(ERT_ALIGN_SIZE)

/*空闲线程栈*/
static ert_uint8_t ert_idle_thread_stack[IDLE_THREAD_STACK_SIZE];

/*空闲线程的线程控制块*/
struct ert_thread idle;

extern ert_uint32_t thread_num;
/*
*brief:空闲线程初始化
*/
void ert_thread_idle_init(void)
{
    thread_num--;
    /*初始化线程*/
    ert_thread_init(&idle,
                    ert_thread_idle_entry,
                    ERT_NULL,
                    &ert_idle_thread_stack[0],
                    sizeof(ert_idle_thread_stack),
                    ERT_THREAD_PRIORITY_MAX-1);

}