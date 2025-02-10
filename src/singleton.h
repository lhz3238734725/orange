#ifndef __ORANGE_SINGLETON_H__
#define __ORANGE_SINGLETON_H__

#include <memory>

namespace orange {

/**
 * @brief 单例模式封装类
 * @details T 类型
 *          X 为了创造多个示例对应的Tag
 *          N 同一个Tag创造多个示例索引
 */
template <class T, class X = void, int N = 0>
class Singleton
{
public:
    static T* GetInstance(){
        static T instance;
        return &instance;
    }
};

/**
 * @brief 单例模式智能指针封装类
 * @details T 类型
 *          X 为了创造多个示例对应的Tag
 *          N 同一个Tag创造多个示例索引
 */
template <class T, class X = void, int N = 0>
class SingletonPtr
{
public:
    static std::shared_ptr<T> GetInstance(){
        static std::shared_ptr<T> instancePtr;
        return instancePtr;
    }
};

}

#endif