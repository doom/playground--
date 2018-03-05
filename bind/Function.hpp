
#ifndef PROJECT_FUNCTION_HPP
#define PROJECT_FUNCTION_HPP

#include "pp/PP.hpp"

template <typename T>
class Function
{
public:
    struct AbstractCallable
    {
    };

    template <typename Functor>
    struct Callable : AbstractCallable
    {
    };
};

template <typename RetT>
class Function<RetT()>
{
private:
    struct AbstractCallable
    {
        virtual ~AbstractCallable()
        {
        }

        virtual RetT operator()() = 0;

        virtual AbstractCallable *clone() const = 0;
    };

    template <typename Functor>
    struct Callable : AbstractCallable
    {
        Callable(const Functor &f) : _f(f)
        {}

        virtual RetT operator()()
        {
            return _f();
        }

        virtual AbstractCallable *clone() const
        {
            return new Callable(_f);
        }

    private:
        Functor _f;
    };

public:
    Function() : _func(NULL)
    {
    }

    template <typename Functor>
    Function(const Functor &f) : _func(new Callable<Functor>(f))
    {
    }

    ~Function()
    {
        delete _func;
    }

    template <typename Functor>
    Function &operator=(const Functor &f)
    {
        delete _func;
        _func = new Callable<Functor>(f);
        return *this;
    }

    RetT operator()()
    {
        return (*_func)();
    }

    typedef RetT result_type;

private:
    AbstractCallable *_func;
};

#define __PUT_ARG_TYPENAME(n)   typename Arg##n
#define __PUT_ARG_TYPE(n)       Arg##n
#define __PUT_ARG_VARNAME(n)    arg##n
#define __PUT_ARG_FULL(n)       __PUT_ARG_TYPE(n) __PUT_ARG_VARNAME(n)

#define GENERATE_FUNCTOR(...)                                                           \
    template <typename RetT, pp_for_each(__PUT_ARG_TYPENAME, __VA_ARGS__)>              \
    class Function<RetT(pp_for_each(__PUT_ARG_TYPE, __VA_ARGS__))>                      \
    {                                                                                   \
    private:                                                                            \
        struct AbstractCallable                                                         \
        {                                                                               \
            virtual ~AbstractCallable()                                                 \
            {                                                                           \
            }                                                                           \
                                                                                        \
            virtual RetT operator()(pp_for_each(__PUT_ARG_FULL, __VA_ARGS__)) = 0;      \
                                                                                        \
            virtual AbstractCallable *clone() const = 0;                                \
        };                                                                              \
                                                                                        \
        template <typename Functor>                                                     \
        struct Callable : AbstractCallable                                              \
        {                                                                               \
            Callable(const Functor &f) : _f(f)                                          \
            {                                                                           \
            }                                                                           \
                                                                                        \
            virtual RetT operator()(pp_for_each(__PUT_ARG_FULL, __VA_ARGS__))           \
            {                                                                           \
                return _f(pp_for_each(__PUT_ARG_VARNAME, __VA_ARGS__));                 \
            }                                                                           \
                                                                                        \
            virtual AbstractCallable *clone() const                                     \
            {                                                                           \
                return new Callable(_f);                                                \
            }                                                                           \
                                                                                        \
        private:                                                                        \
            Functor _f;                                                                 \
        };                                                                              \
                                                                                        \
    public:                                                                             \
        Function() : _func(NULL)                                                        \
        {                                                                               \
        }                                                                               \
                                                                                        \
        template <typename Functor>                                                     \
        Function(const Functor &f) : _func(new Callable<Functor>(f))                    \
        {                                                                               \
        }                                                                               \
                                                                                        \
        Function(const Function &f) : _func(f._func->clone())                           \
        {                                                                               \
        }                                                                               \
                                                                                        \
        ~Function()                                                                     \
        {                                                                               \
            delete _func;                                                               \
        }                                                                               \
                                                                                        \
        template <typename Functor>                                                     \
        Function &operator=(const Functor &f)                                           \
        {                                                                               \
            delete _func;                                                               \
            _func = new Callable<Functor>(f);                                           \
            return *this;                                                               \
        }                                                                               \
                                                                                        \
        Function &operator=(const Function &f)                                          \
        {                                                                               \
            delete _func;                                                               \
            _func = f._func->clone();                                                   \
            return *this;                                                               \
        }                                                                               \
                                                                                        \
        RetT operator()(pp_for_each(__PUT_ARG_FULL, __VA_ARGS__))                       \
        {                                                                               \
            return (*_func)(pp_for_each(__PUT_ARG_VARNAME, __VA_ARGS__));               \
        }                                                                               \
                                                                                        \
        typedef RetT result_type;                                                       \
                                                                                        \
    private:                                                                            \
        AbstractCallable *_func;                                                        \
    };                                                                                  \


GENERATE_FUNCTOR(1);
GENERATE_FUNCTOR(1, 2);
GENERATE_FUNCTOR(1, 2, 3);
GENERATE_FUNCTOR(1, 2, 3, 4);

#undef __PUT_ARG_TYPENAME
#undef __PUT_ARG_TYPE
#undef __PUT_ARG_VARNAME
#undef __PUT_ARG_FULL

#endif //PROJECT_FUNCTION_HPP
