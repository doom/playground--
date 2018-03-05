
#ifndef TEST_BIND_MEMBERFUNC_HPP
#define TEST_BIND_MEMBERFUNC_HPP

#define __PUT_TYPENAME(n)   typename P##n
#define __PUT_TYPE(n)       P##n
#define __PUT_ARG(n)        p##n
#define __PUT_FULLARG(n)    __PUT_TYPE(n) __PUT_ARG(n)

template <typename Ret, typename Object>
class MemberFunc0
{
public:
    MemberFunc0(Object &obj, Ret(Object::*f)()) : _obj(&obj), _func(f)
    {
    }

    Ret operator()()
    {
        return (_obj->*_func)();
    }

private:
    Object *_obj;
    Ret (Object::*_func)();
};

#define GENERATE_MEMBER_FUNC(...)                                                                   \
    template <typename Ret, typename Object, pp_for_each(__PUT_TYPENAME, __VA_ARGS__)>              \
    class pp_paste(MemberFunc, pp_count_args(__VA_ARGS__))                                          \
    {                                                                                               \
    public:                                                                                         \
        typedef Ret(Object::*MemberFuncType)(pp_for_each(__PUT_TYPE, __VA_ARGS__));                 \
                                                                                                    \
        pp_paste(MemberFunc, pp_count_args(__VA_ARGS__))(Object &obj, MemberFuncType f) :           \
                 _obj(&obj), _func(f)                                                               \
        {                                                                                           \
        }                                                                                           \
                                                                                                    \
        Ret operator()(pp_for_each(__PUT_FULLARG, __VA_ARGS__))                                     \
        {                                                                                           \
            return (_obj->*_func)(pp_for_each(__PUT_ARG, __VA_ARGS__));                             \
        }                                                                                           \
                                                                                                    \
    private:                                                                                        \
        Object *_obj;                                                                               \
        MemberFuncType _func;                                                                       \
    };

GENERATE_MEMBER_FUNC(1);
GENERATE_MEMBER_FUNC(1, 2);
GENERATE_MEMBER_FUNC(1, 2, 3);
GENERATE_MEMBER_FUNC(1, 2, 3, 4);
GENERATE_MEMBER_FUNC(1, 2, 3, 4, 5);
GENERATE_MEMBER_FUNC(1, 2, 3, 4, 5, 6);

template <typename Ret, typename Object>
class ConstMemberFunc0
{
public:
    ConstMemberFunc0(Object &obj, Ret(Object::*f)() const) : _obj(&obj), _func(f)
    {
    }

    Ret operator()()
    {
        return (_obj->*_func)();
    }

private:
    Object *_obj;
    Ret (Object::*_func)() const;
};

#define GENERATE_CONST_MEMBER_FUNC(...)                                                             \
    template <typename Ret, typename Object, pp_for_each(__PUT_TYPENAME, __VA_ARGS__)>              \
    class pp_paste(ConstMemberFunc, pp_count_args(__VA_ARGS__))                                     \
    {                                                                                               \
    public:                                                                                         \
        typedef Ret(Object::*MemberFuncType)(pp_for_each(__PUT_TYPE, __VA_ARGS__)) const;           \
                                                                                                    \
        pp_paste(ConstMemberFunc, pp_count_args(__VA_ARGS__))(Object &obj, MemberFuncType f) :      \
                 _obj(&obj), _func(f)                                                               \
        {                                                                                           \
        }                                                                                           \
                                                                                                    \
        Ret operator()(pp_for_each(__PUT_FULLARG, __VA_ARGS__))                                     \
        {                                                                                           \
            return (_obj->*_func)(pp_for_each(__PUT_ARG, __VA_ARGS__));                             \
        }                                                                                           \
                                                                                                    \
    private:                                                                                        \
        Object *_obj;                                                                               \
        MemberFuncType _func;                                                                       \
    };

GENERATE_CONST_MEMBER_FUNC(1);
GENERATE_CONST_MEMBER_FUNC(1, 2);
GENERATE_CONST_MEMBER_FUNC(1, 2, 3);
GENERATE_CONST_MEMBER_FUNC(1, 2, 3, 4);
GENERATE_CONST_MEMBER_FUNC(1, 2, 3, 4, 5);
GENERATE_CONST_MEMBER_FUNC(1, 2, 3, 4, 5, 6);

#undef GENERATE_MEMBER_FUNC
#undef GENERATE_CONST_MEMBER_FUNC
#undef __PUT_FULLARG
#undef __PUT_ARG
#undef __PUT_TYPE
#undef __PUT_TYPENAME

#endif //TEST_BIND_MEMBERFUNC_HPP
