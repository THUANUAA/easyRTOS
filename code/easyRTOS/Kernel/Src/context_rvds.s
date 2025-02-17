//*********************************
//           全局变量
//*********************************
.extern ert_thread_switch_interrupt_flag
.extern ert_interrupt_from_thread
.extern ert_interrupt_to_thread
.extern ert_interrupt_current_thread
// *********************************
//           数据段定义
// *********************************
.data
// 定义全局变量并初始化
ert_thread_switch_interrupt_flag:    .word 0  // 初始化为 0
ert_interrupt_from_thread:           .word 0  // 初始化为 0
ert_interrupt_to_thread:             .word 0  // 初始化为 0
ert_interrupt_current_thread:        .word 0  // 初始化为 0
//*********************************
//             常量
//*********************************
// 系统控制块外设SCB地址范围：0xE000ED00-0xE000ED3F
.equ SCB_VTOR, 0xE000ED08      // 向量表偏移寄存器
.equ NVIC_INT_CTRL, 0xE000ED04      // 中断控制状态寄存器
.equ NVIC_SYSPRI2, 0xE000ED20      // 系统优先级寄存器(2)
.equ NVIC_PENDSV_PRI, 0xFFFF0000      // PendSV 优先级值(lowest)
.equ NVIC_PENDSVSET, 0x10000000      // 触发 PendSV exception 的值

//*********************************
//           代码产生指令
//*********************************
.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb
.text

.global ert_hw_context_switch_to
.global PendSV_Handler
.global ert_hw_context_switch

//***********************************
// 函数原型：void ert_hw_context_switch_to(ert_uint32_t to)//
// R0 ---> to
// 该函数用于开启第一次线程切换
//***********************************
.global ert_hw_context_switch_to
.type ert_hw_context_switch_to, %function
ert_hw_context_switch_to:
    // 设置 ert_interrupt_to_thread 的值
    LDR R1, =ert_interrupt_to_thread
    STR R0, [R1]

    // 设置 ert_interrupt_from_thread 的值为0（启动第一次线程切换）
    LDR R1, =ert_interrupt_from_thread
    MOV R0, #0x0
    STR R0, [R1]

    // 设置 ert_interrupt_current_thread 的值为0（启动第一次线程切换）
    LDR R1, =ert_interrupt_current_thread
    STR R0, [R1]

    // 设置中断标志位 ert_thread_switch_interrupt_flag 的值为1
    LDR R1, =ert_thread_switch_interrupt_flag
    MOV R0, #1
    STR R0, [R1]

    // 设置 PendSV 异常的优先级
    LDR R0, =NVIC_SYSPRI2
    LDR R1, =NVIC_PENDSV_PRI
    LDR R2, [R0, #0x00] // 读取
    ORR R1, R1, R2      // 修改
    STR R1, [R0]         // 写入

    // 触发 PendSV 异常（产生上下文切换）
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]

    // 开启中断
    CPSIE   F
    CPSIE   I

    // 永远不会到这里
    BX LR
//***********************************
// 实现线程上下文切换
// void PendSV_Handler(void)//
// R0 ---> switch from thread stack
// R1 ---> switch to thread stack
// psr, pc, LR, R12, R3, R2, R1, R0 are pushed into [from] stack
//***********************************
.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    // 禁用中断，以保护上下文切换不被中断
    MRS R2, PRIMASK
    CPSID I

    // 获取中断标志位
    LDR R0, =ert_thread_switch_interrupt_flag
    LDR R1, [R0]
    // 判断 R1 是否为0，如果为0，则跳转到 pendsv_exit
    CBZ R1, pendsv_exit

    // 如果 R1 不为0，则清除它
    MOV R1, #0
    STR R1, [R0]

    // 判断 ert_interrupt_from_thread 的值是否为0
    LDR R0, =ert_interrupt_from_thread
    LDR R1, [R0]
    // 如果 R1 为0，则跳转到 switch_to_thread
    CBZ R1, switch_to_thread

//===================== 上文保存 =======================
    // 保存线程上下文（寄存器 R4~R11）
    MRS R1, psp
    STMFD R1!, {R4 - R11}
    LDR R0, [R0]
    STR R1, [R0]

//===================== 下文切换 =======================
switch_to_thread:
    // 加载 ert_interrupt_to_thread 地址到 R1
    LDR R1, =ert_interrupt_to_thread
    LDR R0, =ert_interrupt_current_thread
    LDR R1, [R1]
    STR R1, [R0]
    LDR R1, [R1]

    // 从线程栈中恢复寄存器 R4~R11
    LDMFD R1!, {R4 - R11}
    MSR psp, R1

pendsv_exit:
    // 恢复中断
    MSR PRIMASK, R2

    // 确保异常返回时使用 PSP（LR 寄存器的位 2 设置为 1）
    ORR LR, LR, #0x04
    BX LR
//***********************************
// void ert_hw_context_switch(ert_uint32_t from, ert_uint32_t to)//
// R0 ---> from
// R1 ---> to
//***********************************
.global ert_hw_context_switch
.type ert_hw_context_switch, %function
ert_hw_context_switch:
    // 设置中断标志位 ert_thread_switch_interrupt_flag 为1
    LDR R2, =ert_thread_switch_interrupt_flag
    LDR R3, [R2]
    CMP R3, #1
    BEQ _reswitch
    MOV R3, #1
    STR R3, [R2]

    // 设置 ert_interrupt_from_thread 的值
    LDR R2, =ert_interrupt_from_thread
    STR R0, [R2]

_reswitch:
    // 设置 ert_interrupt_to_thread 的值
    LDR R2, =ert_interrupt_to_thread
    STR R1, [R2]

    // 触发 PendSV 异常实现上下文切换
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]
    BX LR