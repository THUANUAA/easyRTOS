#ifndef _TIMER_H
#define _TIMER_H

#include "ert_typedef.h"

#define  ERT_TIMER_STOP            0x0 /*定时器停止状态*/
#define  ERT_TIMER_ACTIVATE        0x1 /*定时器活动状态*/
#define  ERT_TIMER_ONE_SHOT        0x0 /*单次计时*/
#define  ERT_TIMER_PERIODIC        0x2 /*周期计时*/

struct ert_timer
{
    /* data */
    ert_uint32_t  start_time;     /*定时器开始计时时间*/
    ert_uint32_t  current_time;   /*定时器当前时刻时间*/
    ert_uint32_t  target_time;    /*定时器目标时间*/
    ert_bool_t  timer_status;   /*定时器状态*/
    void  (*timeout_func)(void *parameter); /*超时回调函数*/
    void  *parameter;           /*超时回调函数形参*/

};
typedef struct ert_timer* ert_timer_t;

extern struct ert_timer system_timer;/*系统定时器*/


void system_timer_init(ert_timer_t timer,
    void  (*timeout_func)(void *parameter),
    void  *parameter,
    ert_bool_t timer_status);
void ert_timer_init(ert_timer_t timer,
    void  (*timeout_func)(void *parameter),
    void  *parameter,
    ert_bool_t timer_status);
void ert_timer_start(ert_timer_t timer,ert_tick_t time);
void ert_timer_stop(ert_timer_t timer);

#endif // !_TIMER_H