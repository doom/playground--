
#ifndef TEST_BIND_CALLER_HPP
#define TEST_BIND_CALLER_HPP

#include <pp/PP.hpp>
#include "Storage.hpp"
#include "TypeList.hpp"
#include "TypeTraits.hpp"
#include "MemberFunc.hpp"

template <typename ReturnType, typename Callable, typename List>
class Caller
{
public:
    Caller(Callable c, List l) : _f(c), _l(l)
    {
    }

    ReturnType operator()()
    {
        TypeList0 l0;
        return _l(Type<ReturnType>(), _f, l0);
    }

    typedef ReturnType result_type;

#define __PUT_TYPENAME(n)   typename T##n
#define __PUT_TYPE(n)       T##n
#define __PUT_ARG(n)        t##n
#define __PUT_FULLARG(n)    __PUT_TYPE(n) __PUT_ARG(n)

#define GENERATE_CALL_OP(...)                                                                               \
    template <pp_for_each(__PUT_TYPENAME, __VA_ARGS__)>                                                     \
    ReturnType operator()(pp_for_each(__PUT_FULLARG, __VA_ARGS__))                                          \
    {                                                                                                       \
        pp_paste(TypeList, pp_count_args(__VA_ARGS__))                                                      \
                 <pp_for_each(__PUT_TYPE, __VA_ARGS__)> l(pp_for_each(__PUT_ARG, __VA_ARGS__));             \
        return _l(Type<ReturnType>(), _f, l);                                                               \
    }

    GENERATE_CALL_OP(1);
    GENERATE_CALL_OP(1, 2);
    GENERATE_CALL_OP(1, 2, 3);
    GENERATE_CALL_OP(1, 2, 3, 4);
    GENERATE_CALL_OP(1, 2, 3, 4, 5);
    GENERATE_CALL_OP(1, 2, 3, 4, 5, 6);

#undef GENERATE_CALL_OP
#undef __PUT_FULLARG
#undef __PUT_ARG
#undef __PUT_TYPE
#undef __PUT_TYPENAME

private:
    Callable _f;
    List _l;
};

#define __PUT_ID(x)             x
#define ListTraitsList(...)                                                                                 \
    typename pp_paste(ListTraits, pp_count_args(__VA_ARGS__))<pp_for_each(__PUT_ID, __VA_ARGS__)>::List

#define __PUT_FP_TYPENAME(n)    typename FParam##n
#define __PUT_FP_TYPE(n)        FParam##n
#define __PUT_P_TYPENAME(n)     typename Param##n
#define __PUT_P_TYPE(n)         Param##n
#define __PUT_P_ARG(n)          p##n
#define __PUT_P_FULLARG(n)      __PUT_P_TYPE(n) __PUT_P_ARG(n)

template <typename Ret>
Caller<Ret, Ret(*)(), TypeList0> bind(Ret(*f)())
{
    return Caller<Ret, Ret(*)(), TypeList0>(f, TypeList0());
}

#define GENERATE_DEFAULT_BIND(...)                                                                              \
    template <typename Ret,                                                                                     \
    pp_for_each(__PUT_FP_TYPENAME, __VA_ARGS__), pp_for_each(__PUT_P_TYPENAME, __VA_ARGS__)>                    \
    Caller<Ret, Ret(*)(pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)),                                                \
           ListTraitsList(pp_for_each(__PUT_P_TYPE, __VA_ARGS__))>                                              \
bind(Ret(*f)(pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)), pp_for_each(__PUT_P_FULLARG, __VA_ARGS__))               \
{                                                                                                               \
    typedef Ret(*F)(pp_for_each(__PUT_FP_TYPE, __VA_ARGS__));                                                   \
    typedef pp_paste(ListTraits, pp_count_args(__VA_ARGS__))<pp_for_each(__PUT_P_TYPE, __VA_ARGS__)> LT;        \
    typename LT::List l(pp_for_each(__PUT_P_ARG, __VA_ARGS__));                                                 \
    return Caller<Ret, F, typename LT::List>(f, l);                                                             \
}

GENERATE_DEFAULT_BIND(1);
GENERATE_DEFAULT_BIND(1, 2);
GENERATE_DEFAULT_BIND(1, 2, 3);
GENERATE_DEFAULT_BIND(1, 2, 3, 4);
GENERATE_DEFAULT_BIND(1, 2, 3, 4, 5);
GENERATE_DEFAULT_BIND(1, 2, 3, 4, 5, 6);

//Member functions
template <typename Ret, typename Object>
Caller<Ret, MemberFunc0<Ret, Object>, TypeList0> bind(Ret(Object::*f)(), Object &o)
{
    typedef MemberFunc0<Ret, Object> MF;
    return Caller<Ret, MF, TypeList0>(MF(o, f), TypeList0());
}

#define GENERATE_MF_BIND(...)                                                                                   \
    template <typename Ret, typename Object,                                                                    \
    pp_for_each(__PUT_FP_TYPENAME, __VA_ARGS__), pp_for_each(__PUT_P_TYPENAME, __VA_ARGS__)>                    \
    Caller<Ret, pp_paste(MemberFunc, pp_count_args(__VA_ARGS__))<Ret, Object,                                   \
           pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)>,                                                            \
           ListTraitsList(pp_for_each(__PUT_P_TYPE, __VA_ARGS__))>                                              \
bind(Ret(Object::*f)(pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)), Object &o,                                       \
     pp_for_each(__PUT_P_FULLARG, __VA_ARGS__))                                                                 \
{                                                                                                               \
    typedef pp_paste(MemberFunc, pp_count_args(__VA_ARGS__))<Ret, Object,                                       \
                     pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)> MF;                                               \
    typedef pp_paste(ListTraits, pp_count_args(__VA_ARGS__))<pp_for_each(__PUT_P_TYPE, __VA_ARGS__)> LT;        \
    typename LT::List l(pp_for_each(__PUT_P_ARG, __VA_ARGS__));                                                 \
    return Caller<Ret, MF, typename LT::List>(MF(o, f), l);                                                     \
}

GENERATE_MF_BIND(1);
GENERATE_MF_BIND(1, 2);
GENERATE_MF_BIND(1, 2, 3);
GENERATE_MF_BIND(1, 2, 3, 4);
GENERATE_MF_BIND(1, 2, 3, 4, 5);
GENERATE_MF_BIND(1, 2, 3, 4, 5, 6);

template <typename Ret, typename Object>
Caller<Ret, ConstMemberFunc0<Ret, Object>, TypeList0> bind(Ret(Object::*f)() const, Object &o)
{
    typedef ConstMemberFunc0<Ret, Object> CMF;
    return Caller<Ret, CMF, TypeList0>(CMF(o, f), TypeList0());
}

#define GENERATE_CMF_BIND(...)                                                                                  \
    template <typename Ret, typename Object,                                                                    \
    pp_for_each(__PUT_FP_TYPENAME, __VA_ARGS__), pp_for_each(__PUT_P_TYPENAME, __VA_ARGS__)>                    \
    Caller<Ret, pp_paste(ConstMemberFunc, pp_count_args(__VA_ARGS__))<Ret, Object,                              \
           pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)>,                                                            \
           ListTraitsList(pp_for_each(__PUT_P_TYPE, __VA_ARGS__))>                                              \
bind(Ret(Object::*f)(pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)) const, Object &o,                                 \
     pp_for_each(__PUT_P_FULLARG, __VA_ARGS__))                                                                 \
{                                                                                                               \
    typedef pp_paste(ConstMemberFunc, pp_count_args(__VA_ARGS__))<Ret, Object,                                  \
                     pp_for_each(__PUT_FP_TYPE, __VA_ARGS__)> MF;                                               \
    typedef pp_paste(ListTraits, pp_count_args(__VA_ARGS__))<pp_for_each(__PUT_P_TYPE, __VA_ARGS__)> LT;        \
    typename LT::List l(pp_for_each(__PUT_P_ARG, __VA_ARGS__));                                                 \
    return Caller<Ret, MF, typename LT::List>(MF(o, f), l);                                                     \
}

GENERATE_CMF_BIND(1);
GENERATE_CMF_BIND(1, 2);
GENERATE_CMF_BIND(1, 2, 3);
GENERATE_CMF_BIND(1, 2, 3, 4);
GENERATE_CMF_BIND(1, 2, 3, 4, 5);
GENERATE_CMF_BIND(1, 2, 3, 4, 5, 6);

//Function objects

#define GENERATE_FUNCTOR_BIND(...)                                                                              \
    template <typename Fctor, pp_for_each(__PUT_P_TYPENAME, __VA_ARGS__)>                                       \
    Caller<typename Fctor::result_type, Fctor, ListTraitsList(pp_for_each(__PUT_P_TYPE, __VA_ARGS__))>          \
bind(Fctor f, pp_for_each(__PUT_P_FULLARG, __VA_ARGS__))                                                        \
{                                                                                                               \
    typedef pp_paste(ListTraits, pp_count_args(__VA_ARGS__))<pp_for_each(__PUT_P_TYPE, __VA_ARGS__)> LT;        \
    typename LT::List l(pp_for_each(__PUT_P_ARG, __VA_ARGS__));                                                 \
    return Caller<typename Fctor::result_type, Fctor, typename LT::List>(f, l);                                 \
}

GENERATE_FUNCTOR_BIND(1);
GENERATE_FUNCTOR_BIND(1, 2);
GENERATE_FUNCTOR_BIND(1, 2, 3);
GENERATE_FUNCTOR_BIND(1, 2, 3, 4);
GENERATE_FUNCTOR_BIND(1, 2, 3, 4, 5);
GENERATE_FUNCTOR_BIND(1, 2, 3, 4, 5, 6);

#undef GENERATE_FUNCTOR_BIND
#undef GENERATE_MF_BIND
#undef GENERATE_CMF_BIND
#undef GENERATE_DEFAULT_BIND

#undef __PUT_FP_TYPENAME
#undef __PUT_FP_TYPE
#undef __PUT_P_TYPENAME
#undef __PUT_P_TYPE
#undef __PUT_P_ARG
#undef __PUT_P_FULLARG

#undef __PUT_ID
#undef ListTraitsList

#endif //TEST_BIND_CALLER_HPP
