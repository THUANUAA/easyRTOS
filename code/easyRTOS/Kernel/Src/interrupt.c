#include "interrupt.h"
#include "ert_thread.h"
#include "ethw.h"

/*中断计数器*/
volatile ert_uint8_t ert_interrupt_nest;

/*
*brief: 当BSP文件的中断服务函数进入时会调用该函数
*/
void ert_interrupt_enter(void)
{
    ert_int32_t level;

    /*关中断*/
    level=ert_hw_interrupt_disable();
    /*中断计数器加1*/
    ert_interrupt_nest++;
    /*开中断*/
    ert_hw_interrupt_enable(level);
}

/*
*brief: 当BSP文件的中断服务函数退出时会调用该函数
*/
void ert_interrupt_leave(void)
{
    ert_int32_t level;

    /*关中断*/
    level=ert_hw_interrupt_disable();

    /*中断计数器减1*/
    ert_interrupt_nest--;

    /*开中断*/
    ert_hw_interrupt_enable(level);
}
