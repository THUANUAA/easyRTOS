#include "easy_rtos.h"
#include "main.h"
#include "gpio.h"
#include "core_cm3.h"

/* 定义线程控制块 */ 
struct ert_thread ert_flag1_thread;
struct ert_thread ert_flag2_thread;
struct ert_thread ert_flag3_thread;

ALIGN(ERT_ALIGN_SIZE)
/* 定义线程栈 */ 
#define THREAD_STACK_SIZE                         1024
ert_uint8_t ert_flag1_thread_stack[THREAD_STACK_SIZE];
ert_uint8_t ert_flag2_thread_stack[THREAD_STACK_SIZE];
ert_uint8_t ert_flag3_thread_stack[THREAD_STACK_SIZE];

void easy_rtos_init(void)
{
    /*关中断*/
    ert_hw_interrupt_disable();
    /*Systick中断频率设置*/
    SysTick_Config(SystemCoreClock/ERT_TICK_PRE_SECOND);

    /* 调度器初始化 */
    ert_system_scheduler_init();

    /* 空闲线程初始化*/
    ert_thread_idle_init();

    ert_thread_init(&ert_flag1_thread,              /*线程控制块*/
                    flag1_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag1_thread_stack[0],     /*线程栈起始地址*/
                    sizeof(ert_flag1_thread_stack), /*线程栈大小，单位为字节*/
                    0
                    );

    ert_thread_init(&ert_flag2_thread,              /*线程控制块*/
                    flag2_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag2_thread_stack[0],     /*线程栈起始地址*/
                    sizeof(ert_flag2_thread_stack), /*线程栈大小，单位为字节*/
                    0
                    );

    ert_thread_init(&ert_flag3_thread,              /*线程控制块*/
                    flag3_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag3_thread_stack[1],     /*线程栈起始地址*/
                    sizeof(ert_flag3_thread_stack), /*线程栈大小，单位为字节*/
                    0
                    );

    /*启动系统调度器*/
    ert_system_scheduler_start();
}

void flag1_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET); 
        ert_thread_delay(100);
    }
}

void flag2_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET); 
        ert_thread_delay(200);
    }
}

void flag3_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET); 
        ert_thread_delay(300);
    }
}

/*
* brief: 空闲线程函数
* param: 参数
*/
void ert_thread_idle_entry(void *arg)
{
    (void)arg;
    while(1)
    {
        HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_RESET);
    }
}

void SysTick_Handler(void)
{
  /*开中断*/
  ert_interrupt_enter();
  /*时基更新*/
  ert_tick_increase();
  /*关中断*/
  ert_interrupt_leave();
}