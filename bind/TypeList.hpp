
#ifndef TEST_BIND_TYPELIST_HPP
#define TEST_BIND_TYPELIST_HPP

#include "Storage.hpp"

template <size_t i>
struct placeholder
{
};

placeholder<1> _1;
placeholder<2> _2;
placeholder<3> _3;
placeholder<4> _4;
placeholder<5> _5;
placeholder<6> _6;

#define GENERATE_BRACKET_OP()                                               \
    template <typename T>                                                   \
    T &operator[](Value<T> &value)                                          \
    {                                                                       \
        return value.get();                                                 \
    }

class TypeList0 : Storage0
{
public:
    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &)
    {
        return caller();
    }
};

template <typename T1>
class TypeList1 : Storage1<T1>
{
private:
    typedef Storage1<T1> Parent;

public:
    explicit TypeList1(const T1 &t1) : Parent(t1)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1]);
    }
};

template <typename T1, typename T2>
class TypeList2 : Storage2<T1, T2>
{
private:
    typedef Storage2<T1, T2> Parent;

public:
    TypeList2(const T1 &t1, const T2 &t2) : Parent(t1, t2)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    T2 &operator[](Value<placeholder<2> >)
    {
        return Parent::_t2;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1], list[Parent::_t2]);
    }
};

template <typename T1, typename T2, typename T3>
class TypeList3 : Storage3<T1, T2, T3>
{
    typedef Storage3<T1, T2, T3> Parent;

public:

    TypeList3(const T1 &t1, const T2 &t2, const T3 &t3) : Parent(t1, t2, t3)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    T2 &operator[](Value<placeholder<2> >)
    {
        return Parent::_t2;
    }

    T3 &operator[](Value<placeholder<3> >)
    {
        return Parent::_t3;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1], list[Parent::_t2], list[Parent::_t3]);
    }
};

template <typename T1, typename T2, typename T3, typename T4>
class TypeList4 : Storage4<T1, T2, T3, T4>
{
    typedef Storage4<T1, T2, T3, T4> Parent;

public:
    TypeList4(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4) : Parent(t1, t2, t3, t4)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    T2 &operator[](Value<placeholder<2> >)
    {
        return Parent::_t2;
    }

    T3 &operator[](Value<placeholder<3> >)
    {
        return Parent::_t3;
    }

    T4 &operator[](Value<placeholder<4> >)
    {
        return Parent::_t4;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1], list[Parent::_t2], list[Parent::_t3], list[Parent::_t4]);
    }
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class TypeList5 : Storage5<T1, T2, T3, T4, T5>
{
private:
    typedef Storage5<T1, T2, T3, T4, T5> Parent;

public:
    TypeList5(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5) : Parent(t1, t2, t3, t4, t5)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    T2 &operator[](Value<placeholder<2> >)
    {
        return Parent::_t2;
    }

    T3 &operator[](Value<placeholder<3> >)
    {
        return Parent::_t3;
    }

    T4 &operator[](Value<placeholder<4> >)
    {
        return Parent::_t4;
    }

    T5 &operator[](Value<placeholder<5> >)
    {
        return Parent::_t5;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1], list[Parent::_t2], list[Parent::_t3], list[Parent::_t4], list[Parent::_t5]);
    }
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class TypeList6 : Storage6<T1, T2, T3, T4, T5, T6>
{
private:
    typedef Storage6<T1, T2, T3, T4, T5, T6> Parent;

public:
    TypeList6(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5, const T6 &t6) :
        Storage6<T1, T2, T3, T4, T5, T6>(t1, t2, t3, t4, t5, t6)
    {
    }

    T1 &operator[](Value<placeholder<1> >)
    {
        return Parent::_t1;
    }

    T2 &operator[](Value<placeholder<2> >)
    {
        return Parent::_t2;
    }

    T3 &operator[](Value<placeholder<3> >)
    {
        return Parent::_t3;
    }

    T4 &operator[](Value<placeholder<4> >)
    {
        return Parent::_t4;
    }

    T5 &operator[](Value<placeholder<5> >)
    {
        return Parent::_t5;
    }

    T6 &operator[](Value<placeholder<6> >)
    {
        return Parent::_t6;
    }

    GENERATE_BRACKET_OP();

    template <typename Ret, typename Caller, typename List>
    Ret operator()(Type<Ret>, Caller &caller, List &list)
    {
        return caller(list[Parent::_t1], list[Parent::_t2], list[Parent::_t3],
                      list[Parent::_t4], list[Parent::_t5], list[Parent::_t6]);
    }
};

template <typename P1>
struct ListTraits1
{
    typedef Value<P1> Param1;
    typedef TypeList1<Param1> List;
};

template <typename P1, typename P2>
struct ListTraits2
{
    typedef Value<P1> Param1;
    typedef Value<P2> Param2;
    typedef TypeList2<Param1, Param2> List;
};

template <typename P1, typename P2, typename P3>
struct ListTraits3
{
    typedef Value<P1> Param1;
    typedef Value<P2> Param2;
    typedef Value<P3> Param3;
    typedef TypeList3<Param1, Param2, Param3> List;
};

template <typename P1, typename P2, typename P3, typename P4>
struct ListTraits4
{
    typedef Value<P1> Param1;
    typedef Value<P2> Param2;
    typedef Value<P3> Param3;
    typedef Value<P4> Param4;
    typedef TypeList4<Param1, Param2, Param3, Param4> List;
};

template <typename P1, typename P2, typename P3, typename P4, typename P5>
struct ListTraits5
{
    typedef Value<P1> Param1;
    typedef Value<P2> Param2;
    typedef Value<P3> Param3;
    typedef Value<P4> Param4;
    typedef Value<P5> Param5;
    typedef TypeList5<Param1, Param2, Param3, Param4, Param5> List;
};

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct ListTraits6
{
    typedef Value<P1> Param1;
    typedef Value<P2> Param2;
    typedef Value<P3> Param3;
    typedef Value<P4> Param4;
    typedef Value<P5> Param5;
    typedef Value<P6> Param6;
    typedef TypeList6<Param1, Param2, Param3, Param4, Param5, Param6> List;
};

#endif //TEST_BIND_TYPELIST_HPP
