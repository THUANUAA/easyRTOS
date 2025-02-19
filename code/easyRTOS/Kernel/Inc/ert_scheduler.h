#ifndef _ERT_SCHEDULER_H
#define _ERT_SCHEDULER_H

#include "ert_typedef.h"
#include "ert_thread.h"
#include "ert_config.h"
#include "ethw.h"
#include "idle.h"
#include "easy_rtos.h"

/* 已知一个结构体里面的成员地址，反推该结构体的首地址 */
#define ert_container_of(ptr,type,member) \
        ((type*)((char*)(ptr)-(ert_uint32_t)(&((type*)0)->member)))

#define ert_list_entry(node,type,member) \
        ert_container_of(node,type,member)


void ert_system_scheduler_init(void);
void ert_system_scheduler_start(void);
void ert_schedule(void);

#endif // !_ERT_SCHEDULER_H
