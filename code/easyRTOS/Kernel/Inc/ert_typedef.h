#ifndef _ERT_TYPEDEF_H
#define _ERT_TYPEDEF_H

/*
 ********************************
            数据类型
 ********************************
 */

/* easyRTOS 数据类型重定义*/
typedef signed char                     ert_int8_t;
typedef signed short                    ert_int16_t;
typedef signed long                     ert_int32_t;
typedef unsigned char                   ert_uint8_t;
typedef unsigned short                  ert_uint16_t;
typedef unsigned long                   ert_uint32_t;
typedef ert_int8_t                      ert_bool_t;

typedef ert_int32_t                     ert_err_t;
typedef ert_uint32_t                    ert_time_t;
typedef ert_uint32_t                    ert_tick_t;
typedef ert_int32_t                     ert_flag_t;
typedef ert_uint32_t                    ert_size_t;
typedef ert_uint32_t                    ert_dev_t;
typedef ert_int32_t                     ert_off_t;

#define ERT_TRUE                        1
#define ERT_FALSE                       0

/* 错误码重定义 */
#define ERT_EOK                         0   /*no error*/
#define ERT_ERROR                       1   /*a generic error happen*/
#define ERT_ETIMEOUT                    2   /*time out*/
#define ERT_EFULL                       3   /*the resource is full*/
#define ERT_EEMPTY                      4   /*the resource is empty*/
#define ERT_ENOMEM                      5   /*no memort*/
#define ERT_ENOSYS                      6   /*no system*/
#define ERT_EBUSY                       7   /*busy*/
#define ERT_EIO                         8   /*io error*/
#define ERT_EINTR                       9   /*interrupted system call*/
#define ERT_EINVAL                      10  /*invalid argument*/


#define ert_inline                      static __inline
#define ALIGN(n)                        __attribute__((aligned(n)))

#define ERT_ALIGN(size, align)          (((size)+(align)-1) & ~((align)-1))
#define ERT_ALIGN_DOWN(size, align)     ((size)& ~((align)-1))

#define ERT_NULL                        (0)

#endif // !_ERT_TYPEDEF_H