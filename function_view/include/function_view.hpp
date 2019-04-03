/*
** Created by doom on 03/04/19.
*/

#ifndef FUNCTION_VIEW_HPP
#define FUNCTION_VIEW_HPP

#include <utility>

template <typename F>
class function_view;

template <typename Ret, typename ...Args>
class function_view<Ret(Args...)>
{
private:
    void *func_ptr_;
    Ret (*func_call_wrapper_)(void *ptr, Args...);

public:
    template <typename F>
    function_view(const F &f) noexcept :
        func_ptr_((void *)&f),
        func_call_wrapper_(
            [](void *ptr, Args ...args) {
                return (*reinterpret_cast<const F *>(ptr))(std::forward<Args>(args)...);
            }
        )
    {
    }

    template <typename F>
    function_view(F &f) noexcept :
        func_ptr_((void *)&f),
        func_call_wrapper_(
            [](void *ptr, Args ...args) {
                return (*reinterpret_cast<F *>(ptr))(std::forward<Args>(args)...);
            }
        )
    {
    }

    Ret operator()(Args ...args)
    {
        return func_call_wrapper_(func_ptr_, std::forward<Args>(args)...);
    }
};

#endif /* !FUNCTION_VIEW_HPP */
