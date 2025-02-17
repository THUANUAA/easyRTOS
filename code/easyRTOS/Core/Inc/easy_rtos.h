#ifndef _EASY_RTOS_H
#define _EASY_RTOS_H

#include "ert_config.h"
#include "ert_cpuport.h"
#include "ert_thread.h"
#include "ert_typedef.h"
#include "ert_scheduler.h"

void easy_rtos_init(void);
void delay(ert_uint32_t count);
void flag1_thread_entry(void *arg);
void flag2_thread_entry(void *arg);

#endif // !_EASY_RTOS_H