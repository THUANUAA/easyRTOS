#ifndef _EASY_RTOS_H
#define _EASY_RTOS_H

#include "ert_config.h"
#include "ert_cpuport.h"
#include "ert_thread.h"
#include "ert_typedef.h"
#include "ert_scheduler.h"
#include "clock.h"
#include "ert_object.h"
#include "idle.h"
#include "interrupt.h"

void easy_rtos_init(void);
void flag1_thread_entry(void *arg);
void flag2_thread_entry(void *arg);
void flag3_thread_entry(void *arg);
void SysTick_Handler(void);
#endif // !_EASY_RTOS_H