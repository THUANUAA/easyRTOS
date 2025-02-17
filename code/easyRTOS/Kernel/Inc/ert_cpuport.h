#ifndef _ERT_CPUPORT_H
#define _ERT_CPUPORT_H

#include "ert_typedef.h"

struct exception_stack_frame
{
    /* data */
    ert_uint32_t r0;
    ert_uint32_t r1;
    ert_uint32_t r2;
    ert_uint32_t r3;
    ert_uint32_t r12;
    ert_uint32_t lr;
    ert_uint32_t pc;
    ert_uint32_t psr;
};

struct stack_frame
{
    /* data */
    ert_uint32_t r4;
    ert_uint32_t r5;
    ert_uint32_t r6;
    ert_uint32_t r7;
    ert_uint32_t r8;
    ert_uint32_t r9;
    ert_uint32_t r10;
    ert_uint32_t r11;

    struct exception_stack_frame exception_stack_frame;
};

ert_uint8_t *ert_hw_stack_init(void *entry,
    void *parameter,
    ert_uint8_t *stack_addr);

#endif // !_ERT_CPUPORT_H