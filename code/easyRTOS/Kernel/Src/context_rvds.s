//*********************************
//           代码产生指令
//*********************************
.syntax unified      // 使用统一语法模式，适用于ARM和Thumb模式
.cpu cortex-m3       // 指定使用 Cortex-M3 处理器架构
.fpu softvfp         // 使用软件浮点运算单元（不使用硬件浮点）
.thumb               // 启用Thumb指令集以减少代码尺寸
.text                // 以下为代码段，表明后面的内容是执行代码

//*********************************
//           全局变量
//*********************************
.extern ert_thread_switch_interrupt_flag  // 外部声明一个线程切换标志，指示是否需要切换线程
.extern ert_interrupt_from_thread        // 外部声明一个指向“源”线程栈的指针
.extern ert_interrupt_to_thread          // 外部声明一个指向“目标”线程栈的指针

// *********************************
//           数据段定义
// *********************************
.data
// 定义并初始化全局变量
ert_thread_switch_interrupt_flag:    .word 0  // 初始化线程切换标志为0
ert_interrupt_from_thread:           .word 0  // 初始化“源”线程标志为0
ert_interrupt_to_thread:             .word 0  // 初始化“目标”线程标志为0

//*********************************
//             常量
//*********************************
// 定义一些常量地址，用于访问系统控制块和中断控制寄存器
.equ SCB_VTOR, 0xE000ED08      // 向量表偏移寄存器的地址
.equ NVIC_INT_CTRL, 0xE000ED04      // 中断控制状态寄存器的地址
.equ NVIC_SYSPRI2, 0xE000ED20      // 系统优先级寄存器(2)的地址
.equ NVIC_PENDSV_PRI, 0xFFFF0000      // PendSV中断优先级值，设置为最低优先级
.equ NVIC_PENDSVSET, 0x10000000      // 触发PendSV中断的值

//***********************************
// 函数原型：void ert_hw_context_switch_to(ert_uint32_t to)
// R0 ---> to
// 该函数用于启动第一次线程切换，设置目标线程栈并触发上下文切换
//***********************************
.global ert_hw_context_switch_to
.type ert_hw_context_switch_to, %function
ert_hw_context_switch_to:
    // 设置 ert_interrupt_to_thread 的值为目标线程的栈地址
    LDR R1, =ert_interrupt_to_thread    // 加载 ert_interrupt_to_thread 地址到 R1
    STR R0, [R1]                        // 将传入的参数 to 存储到 ert_interrupt_to_thread

    // 设置 ert_interrupt_from_thread 的值为0（表示启动第一次线程切换）
    LDR R1, =ert_interrupt_from_thread   // 加载 ert_interrupt_from_thread 地址到 R1
    MOV R0, #0                          // 将 R0 设置为 0，表示初始化“源”线程
    STR R0, [R1]                        // 将 0 存储到 ert_interrupt_from_thread

    // 设置线程切换标志位 ert_thread_switch_interrupt_flag 为1，表示需要进行线程切换
    LDR R1, =ert_thread_switch_interrupt_flag    // 加载线程切换标志地址
    MOV R0, #1                                // 将 R0 设置为 1，表示需要进行线程切换
    STR R0, [R1]                               // 将 1 存储到 ert_thread_switch_interrupt_flag

    // 设置 PendSV 异常的优先级
    LDR R0, =NVIC_SYSPRI2                   // 加载系统优先级寄存器的地址
    LDR R1, =NVIC_PENDSV_PRI                // 加载 PendSV 优先级值
    LDR R2, [R0, #0x00]                     // 读取当前优先级
    ORR R1, R1, R2                          // 修改优先级，将 PendSV 优先级设为最低
    STR R1, [R0]                            // 将修改后的优先级写回系统优先级寄存器

    // 触发 PendSV 异常（触发上下文切换）
    LDR R0, =NVIC_INT_CTRL                  // 加载中断控制寄存器的地址
    LDR R1, =NVIC_PENDSVSET                 // 加载触发 PendSV 异常的值
    STR R1, [R0]                            // 写入触发 PendSV 的值，开始上下文切换

    // 开启中断
    CPSIE   F                                // 启用浮点异常中断
    CPSIE   I                                // 启用普通中断

    // 永远不会到这里，因为触发PendSV后程序会自动返回
    BX LR
//***********************************
// 实现线程上下文切换
// void PendSV_Handler(void)
// R0 ---> switch from thread stack
// R1 ---> switch to thread stack
// psr, pc, LR, R12, R3, R2, R1, R0 are pushed into [from] stack
//***********************************
.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    // 禁用中断，以防止上下文切换被其他中断干扰
    MRS R2, PRIMASK      // 将当前中断使能状态保存到 R2
    CPSID I              // 禁用普通中断

    // 获取线程切换标志位
    LDR R0, =ert_thread_switch_interrupt_flag
    LDR R1, [R0]
    // 如果标志位为0，跳转到 pendsv_exit，表示没有切换需要继续执行当前线程
    CBZ R1, pendsv_exit

    // 如果标志位不为0，清除该标志位
    MOV R1, #0
    STR R1, [R0]

    // 获取 ert_interrupt_from_thread 是否为0
    LDR R0, =ert_interrupt_from_thread
    LDR R1, [R0]
    // 如果为0，跳转到 switch_to_thread
    CBZ R1, switch_to_thread

//===================== 上文保存 =======================
    // 保存当前线程的上下文（寄存器 R4~R11）
    MRS R1, psp            // 获取当前线程的栈指针（PSP）
    STMFD R1!, {R4 - R11}  // 将 R4~R11 寄存器的值保存到栈中
    LDR R0, [R0]           // 获取 ert_interrupt_from_thread 的值（即源线程栈地址）
    STR R1, [R0]           // 将当前栈指针 R1 保存到 ert_interrupt_from_thread

//===================== 下文切换 =======================
switch_to_thread:
    // 加载目标线程的栈指针
    LDR R1, =ert_interrupt_to_thread
    LDR R1, [R1]           // 加载目标线程栈地址
    LDR R1, [R1]           // 获取目标线程的栈指针

    // 恢复目标线程的上下文（寄存器 R4~R11）
    LDMFD R1!, {R4 - R11}  // 从目标线程栈中恢复 R4~R11 寄存器的值
    MSR PSP, R1            // 设置当前栈指针 PSP 为目标线程的栈指针

pendsv_exit:
    // 恢复中断
    MSR PRIMASK, R2       // 恢复中断使能状态

    // 确保异常返回时使用 PSP（LR 寄存器的位 2 设置为1）
    ORR LR, LR, #0x04     // 设置 LR[2] = 1，确保返回时使用 PSP
    BX LR                 // 异常返回，跳回到线程执行

//***********************************
// void ert_hw_context_switch(ert_uint32_t from, ert_uint32_t to)
// R0 ---> from
// R1 ---> to
//***********************************
.global ert_hw_context_switch
.type ert_hw_context_switch, %function
ert_hw_context_switch:
    // 设置线程切换标志位为1，表示需要进行线程切换
    LDR R2, =ert_thread_switch_interrupt_flag
    LDR R3, [R2]
    CMP R3, #1
    BEQ _reswitch          // 如果标志为1，跳到重新切换代码

    MOV R3, #1             // 将标志设置为1
    STR R3, [R2]           // 将标志存储到内存

    // 设置 ert_interrupt_from_thread 为源线程的栈地址
    LDR R2, =ert_interrupt_from_thread
    STR R0, [R2]           // 存储源线程栈地址

_reswitch:
    // 设置 ert_interrupt_to_thread 为目标线程的栈地址
    LDR R2, =ert_interrupt_to_thread
    STR R1, [R2]           // 存储目标线程栈地址

    // 触发 PendSV 异常实现线程切换
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]           // 写入触发 PendSV 的值，启动上下文切换
    BX LR                  // 返回

//***********************************
// ert_int32_t ert_hw_interrupt_disable()
//***********************************
.global ert_hw_interrupt_disable
.type ert_hw_interrupt_disable, %function
ert_hw_interrupt_disable:

    MRS R0,PRIMASK      //将PRIMASK寄存器的值存到R0，函数被调用时，R0会被作为返回值
    CPSID I             //关闭中断
    BX LR               //返回

//***********************************
// void ert_hw_interrupt_enable(ert_int32_t level)
//***********************************
.global ert_hw_interrupt_enable
.type ert_hw_interrupt_enable, %function
ert_hw_interrupt_enable:

    MSR PRIMASK,R0      //将R0的值存到PRIMASK寄存器，函数被调用时，R0会接收形参
    BX LR               //返回
