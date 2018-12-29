/*
** Created by doom on 27/12/18.
*/

#ifndef STRUCT_UTILS_TRAITS_HPP
#define STRUCT_UTILS_TRAITS_HPP

#include <type_traits>
#include <utility>

namespace su
{
    namespace details
    {
        struct any_type
        {
            template <typename T, typename = std::enable_if_t<!std::is_lvalue_reference_v<T>>>
            operator T &&() const;

            template <typename T, typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
            operator T &() const;
        };

        inline constexpr any_type any_nth_param{};

        template <typename Void, typename T, typename ...Args>
        struct is_brace_constructible_helper : std::false_type
        {
        };

        template <typename T, typename ...Args>
        struct is_brace_constructible_helper<std::void_t<decltype(T{std::declval<Args>()...})>, T, Args...> :
            std::true_type
        {
        };
    }

    template <typename T, typename ...Args>
    struct is_brace_constructible : details::is_brace_constructible_helper<void, T, Args...>
    {
    };

    template <typename T, typename ...Args>
    inline constexpr bool is_brace_constructible_v = is_brace_constructible<T, Args...>::value;

    namespace details
    {
        template <typename T, std::size_t ...Is>
        inline constexpr bool is_constructible_with_n_args_helpers(std::index_sequence<Is...>)
        {
            return is_brace_constructible_v<T, decltype((void)Is, any_nth_param)...>;
        }
    }

    template <typename T, std::size_t N>
    struct is_brace_constructible_with_n_args :
        std::bool_constant<details::is_constructible_with_n_args_helpers<T>(std::make_index_sequence<N>{})>
    {
    };

    template <typename T, std::size_t N>
    inline constexpr bool is_brace_constructible_with_n_args_v = is_brace_constructible_with_n_args<T, N>::value;

    namespace details
    {
#define MAKE_STRUCT_ARITY_ALIAS(n)                                                                                  \
    template <typename T, typename = std::enable_if_t<                                                              \
        is_brace_constructible_with_n_args_v<T, n> &&                                                               \
        !is_brace_constructible_with_n_args_v<T, n + 1>                                                             \
    >>                                                                                                              \
    std::integral_constant<std::size_t, n> struct_arity_helper();

        MAKE_STRUCT_ARITY_ALIAS(0);

        MAKE_STRUCT_ARITY_ALIAS(1);

        MAKE_STRUCT_ARITY_ALIAS(2);

        MAKE_STRUCT_ARITY_ALIAS(3);

        MAKE_STRUCT_ARITY_ALIAS(4);

        MAKE_STRUCT_ARITY_ALIAS(5);

        MAKE_STRUCT_ARITY_ALIAS(6);

        MAKE_STRUCT_ARITY_ALIAS(7);

        MAKE_STRUCT_ARITY_ALIAS(8);

#undef MAKE_STRUCT_ARITY_ALIAS
    }

    template <typename T>
    using struct_arity = decltype(details::struct_arity_helper<T>());

    template <typename T>
    inline constexpr std::size_t struct_arity_v = struct_arity<T>::value;
}

#endif /* !STRUCT_UTILS_TRAITS_HPP */
