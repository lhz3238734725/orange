#ifndef __ORANGE_UTIL_H__
#define __ORANGE_UTIL_H__

#include <iostream>
#include <sys/syscall.h>
#include <unistd.h>

namespace orange{
    /**
     * @brief 获取线程ID
     */
    pid_t GetThreadId();

    /**
     * @brief 获取协程ID
     */
    uint32_t GetFiberId();
}

#endif