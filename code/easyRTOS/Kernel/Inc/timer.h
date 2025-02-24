#ifndef _TIMER_H
#define _TIMER_H

#include "ert_typedef.h"

#define  ERT_TIMER_STOP            0 /*定时器停止状态*/
#define  ERT_TIMER_ACTIVATE        1 /*定时器活动状态*/

struct ert_timer
{
    /* data */
    ert_uint8_t  start_time;     /*定时器开始计时时间*/
    ert_uint8_t  current_time;   /*定时器当前时刻时间*/
    ert_uint8_t  target_time;    /*定时器目标时间*/
    ert_bool_t  timer_status;   /*定时器状态*/
};
typedef struct ert_timer* ert_timer_t;

void ert_timer_start(ert_timer_t timer,ert_tick_t time);
void ert_timer_stop(ert_timer_t timer);

#endif // !_TIMER_H