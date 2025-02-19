#ifndef _IDLE_H
#define _IDLE_H

#include "ert_typedef.h"
#include "ert_config.h"
#include "ethw.h"
#include "ert_thread.h"
#include "ert_scheduler.h"


void ert_thread_idle_entry(void *arg);
void ert_thread_idle_init(void);

#endif // !_IDLE_H
