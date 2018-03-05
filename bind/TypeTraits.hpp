
#ifndef TYPETRAITS_TYPETRAITS_HPP
#define TYPETRAITS_TYPETRAITS_HPP

#include <pp/pp_for_each.hpp>

template <typename T>
class Value
{
public:
    Value(const T &value) : _value(value)
    {
    }

    T &get()
    {
        return _value;
    }

    const T &get() const
    {
        return _value;
    }

private:
    T _value;
};

template <typename T, T v>
struct integral_constant
{
    static const T value = v;
};

struct true_type : integral_constant<bool, true>
{
};

struct false_type : integral_constant<bool, false>
{
};

template <typename T, typename U>
struct is_same : false_type
{
};

template <typename T>
struct is_same<T, T> : true_type
{
};

template <typename T>
struct remove_reference
{
    typedef T type;
};

template <typename T>
struct remove_reference<T &>
{
    typedef T type;
};

template <typename T>
struct remove_const
{
    typedef T type;
};
template <typename T>
struct remove_const<const T>
{
    typedef T type;
};

template <typename T>
struct remove_volatile
{
    typedef T type;
};
template <typename T>
struct remove_volatile<volatile T>
{
    typedef T type;
};

template <typename T>
struct remove_cv
{
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

template <typename T>
struct get_type
{
    typedef T type;
};

template <typename T>
struct Type
{
    typedef T type;
};

template <typename T>
T *addressof(T &arg)
{
    return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

template <typename T>
class reference_wrapper
{
public:
    reference_wrapper(T &ref) : _ptr(addressof(ref))
    {}

    operator const T &() const
    {
        return *_ptr;
    }

    operator T &()
    {
        return *_ptr;
    }

    T &get()
    {
        return *_ptr;
    }

    const T &get() const
    {
        return *_ptr;
    }

private:
    T *_ptr;
};

template <typename T>
inline reference_wrapper<T> ref(T &t)
{
    return reference_wrapper<T>(t);
}

template <typename T>
inline reference_wrapper<const T> cref(const T &t)
{
    return reference_wrapper<const T>(t);
};

template <typename T>
inline reference_wrapper<const T> cref(reference_wrapper<const T> t)
{
    return ref(t.get());
}

template <typename T>
inline reference_wrapper<T> ref(reference_wrapper<T> t)
{
    return ref(t.get());
}

template <typename T>
struct result_of
{
};

template <typename RetT>
struct result_of<RetT()>
{
    typedef RetT type;
};

#define __PUT_ARG_TYPENAME(n)   typename Arg##n
#define __PUT_ARG_TYPE(n)       Arg##n

#define GENERATE_RESULT_OF(...)                                                 \
    template <typename RetT, pp_for_each(__PUT_ARG_TYPENAME, __VA_ARGS__)>      \
    struct result_of<RetT(pp_for_each(__PUT_ARG_TYPE, __VA_ARGS__))>             \
    {                                                                           \
        typedef RetT type;                                                      \
    };

GENERATE_RESULT_OF(1)
GENERATE_RESULT_OF(1, 2)
GENERATE_RESULT_OF(1, 2, 3)
GENERATE_RESULT_OF(1, 2, 3, 4)
GENERATE_RESULT_OF(1, 2, 3, 4, 5)
GENERATE_RESULT_OF(1, 2, 3, 4, 5, 6)

#undef GENERATE_RESULT_OF
#undef __PUT_ARG_TYPENAME
#undef __PUT_ARG_TYPE

#endif //TYPETRAITS_TYPETRAITS_HPP
