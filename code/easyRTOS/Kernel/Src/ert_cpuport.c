#include "ert_cpuport.h"

/* 用于存储上一个线程的栈的sp的指针 */
ert_uint32_t ert_interrupt_from_thread;
/* 用于存储下一个将要运行的线程的栈的sp的指针 */
ert_uint32_t ert_interrupt_to_thread;
/* PendSV中断服务函数执行标志 */
ert_uint32_t ert_thread_switch_interrupr_flag;
/*
*brief 初始化线程栈
*param1: 线程入口
*param2: 线程形参
*param3: 线程栈顶地址-4，在该函数调用的时候传进来的是线程栈的栈顶地址-4
*/
ert_uint8_t *ert_hw_stack_init(void *entry,
                                void *parameter,
                                ert_uint8_t *stack_addr)
{
    struct stack_frame *stack_frame; /*定义一个struct stack_frame类型的结构体指针stack_frame*/
    ert_uint8_t *stk;
    ert_uint32_t i;

    /*获取栈顶指针，ert_hw_stack_init 在调用的时候，传给stack_addr的是(栈顶指针-4)*/
    stk = stack_addr + sizeof(ert_uint32_t);

    /*让stk指针向下8字节对齐*/
    stk = (ert_uint8_t *)ERT_ALIGN_DOWN((ert_uint32_t)stk, 8);

    /*stk指针继续向下移动sizeof(struct stack_frame)个偏移*/
    stk -= sizeof(struct stack_frame);

    /*将stk指针强制转化为stack_frame类型后存到stack_frame*/
    stack_frame = (struct stack_frame *)stk; 

    /*以stack_frame为起始地址，将栈空间里面的sizeof(struct stack_frame)个内存初始化为0xdeadbeef*/
    for(i=0; i<sizeof(struct stack_frame)/sizeof(ert_uint32_t);i++)
    {
        ((ert_uint32_t *)stack_frame)[i] = 0xdeadbeef;
    }

    /* 初始化异常发生时自动保存的寄存器 */
    stack_frame->exception_stack_frame.r0 = (ert_uint32_t)parameter; /*R0 必须为线程形参*/
    stack_frame->exception_stack_frame.r1 = 0;
    stack_frame->exception_stack_frame.r2 = 0;
    stack_frame->exception_stack_frame.r3 = 0;
    stack_frame->exception_stack_frame.r12 = 0;
    stack_frame->exception_stack_frame.lr = 0;
    stack_frame->exception_stack_frame.pc = (ert_uint32_t)entry;   /*R15(PC) 入口地址位：entry*/
    stack_frame->exception_stack_frame.psr = 0x01000000L; /*psr寄存器的第24位必须置为1*/
    
    /* 初始化异常发生时自动保存的寄存器 */
    return stk;
}