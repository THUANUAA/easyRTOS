#ifndef _ETHW_H
#define _ETHW_H

#include "ert_typedef.h"

void ert_hw_context_switch(ert_uint32_t from, ert_uint32_t to);
void ert_hw_context_switch_to(ert_uint32_t to);
void ert_hw_context_switch_interrupt(ert_uint32_t from, ert_uint32_t to);
ert_int32_t ert_hw_interrupt_disable();
void ert_hw_interrupt_enable(ert_int32_t level);

#endif // !_ETHW_H