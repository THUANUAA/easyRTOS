#include "easy_rtos.h"
#include "main.h"
#include "gpio.h"

volatile ert_uint32_t flag1;
volatile ert_uint32_t flag2;

/* 线程就绪列表 */
ert_list_t ert_thread_priority_table[ERT_THREAD_PRIORITY_MAX];

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
    /* 调度器初始化 */
    ert_system_scheduler_init();

    ert_thread_init(&ert_flag1_thread,              /*线程控制块*/
                    "线程一",
                    flag1_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag1_thread_stack[0],     /*线程栈起始地址*/
                    sizeof(ert_flag1_thread_stack)  /*线程栈大小，单位为字节*/
                    );
    /*将线程插入就绪列表中*/
    ert_list_insert_before(&(ert_thread_priority_table[0]),&(ert_flag1_thread.tlist));

    ert_thread_init(&ert_flag2_thread,              /*线程控制块*/
                    "线程二",
                    flag2_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag2_thread_stack[0],     /*线程栈起始地址*/
                    sizeof(ert_flag2_thread_stack)  /*线程栈大小，单位为字节*/
                    );
    /*将线程插入就绪列表中*/
    ert_list_insert_before(&(ert_thread_priority_table[1]),&(ert_flag2_thread.tlist));

    ert_thread_init(&ert_flag3_thread,              /*线程控制块*/
                    "线程三",
                    flag3_thread_entry,             /*线程入口地址*/
                    ERT_NULL,                       /*线程形参*/
                    &ert_flag3_thread_stack[0],     /*线程栈起始地址*/
                    sizeof(ert_flag3_thread_stack)  /*线程栈大小，单位为字节*/
                    );
    /*将线程插入就绪列表中*/
    ert_list_insert_before(&(ert_thread_priority_table[2]),&(ert_flag3_thread.tlist));

    /*启动系统调度器*/
    ert_system_scheduler_start();
}

void delay(ert_uint32_t count)
{
    while(count--);
}

void flag1_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
        HAL_Delay(3000);

        ert_schedule();
    }
}

void flag2_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET); 
        HAL_Delay(3000);

        ert_schedule();
    }
}

void flag3_thread_entry(void *arg)
{
    (void)arg;
    while(1){
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET); 
        HAL_Delay(3000);

        ert_schedule();
    }
}
