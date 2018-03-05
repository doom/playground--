
#ifndef TEST_BIND_STORAGE_HPP
#define TEST_BIND_STORAGE_HPP

#include <pp/PP.hpp>

struct Storage0
{

};

template <typename T1>
struct Storage1 : Storage0
{
    Storage1(const T1 &t1) : _t1(t1)
    {
    }

    T1 _t1;
};

template <typename T1, typename T2>
struct Storage2 : Storage1<T1>
{
    Storage2(const T1 &t1, const T2 &t2) : Storage1<T1>(t1), _t2(t2)
    {
    }

    T2 _t2;
};

template <typename T1, typename T2, typename T3>
struct Storage3 : Storage2<T1, T2>
{
    Storage3(const T1 &t1, const T2 &t2, const T3 &t3) : Storage2<T1, T2>(t1, t2), _t3(t3)
    {
    }

    T3 _t3;
};

template <typename T1, typename T2, typename T3, typename T4>
struct Storage4 : Storage3<T1, T2, T3>
{
    Storage4(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4)
        : Storage3<T1, T2, T3>(t1, t2, t3), _t4(t4)
    {
    }

    T4 _t4;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct Storage5 : Storage4<T1, T2, T3, T4>
{
    Storage5(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5)
        : Storage4<T1, T2, T3, T4>(t1, t2, t3, t4), _t5(t5)
    {
    }

    T5 _t5;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct Storage6 : Storage5<T1, T2, T3, T4, T5>
{
    Storage6(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6)
        : Storage5<T1, T2, T3, T4, T5>(t1, t2, t3, t4, t5), _t6(t6)
    {
    }

    T6 _t6;
};

#endif //TEST_BIND_STORAGE_HPP