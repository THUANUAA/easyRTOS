#include "ert_thread.h"

/*
*brief: 将指定个数的字符串从一个地方拷贝到另外一个地方
*param1: 拷贝的目的地址
*param2: 拷贝的源地址
*param3: 拷贝的长度
*return: 拷贝后的结果
*/
ert_int8_t *ert_strncpy(char *dst,const char *src,ert_uint32_t n)
{
    if(n!=0)
    {
        ert_int8_t *d=dst;
        const ert_int8_t *s=src;

        do
        {
            if((*d++=*s++)==0)
            {
                while(--n!=0)
                    *d++=0;
                break;
            }
        } while (--n!=0);
    }
    return (dst);
}