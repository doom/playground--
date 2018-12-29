/*
** Created by doom on 24/12/18.
*/

#ifndef STRUCT_UTILS_AS_TUPLE_HPP
#define STRUCT_UTILS_AS_TUPLE_HPP

#include <tuple>
#include <struct_utils/traits.hpp>

namespace su
{
    namespace details
    {
        template <typename T>
        constexpr auto as_tuple(T &, std::integral_constant<size_t, 0>) noexcept
        {
            return std::make_tuple();
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 1>) noexcept
        {
            auto &[m1] = t;

            return std::forward_as_tuple(m1);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 2>) noexcept
        {
            auto &[m1, m2] = t;

            return std::forward_as_tuple(m1, m2);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 3>) noexcept
        {
            auto &[m1, m2, m3] = t;

            return std::forward_as_tuple(m1, m2, m3);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 4>) noexcept
        {
            auto &[m1, m2, m3, m4] = t;

            return std::forward_as_tuple(m1, m2, m3, m4);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 5>) noexcept
        {
            auto &[m1, m2, m3, m4, m5] = t;

            return std::forward_as_tuple(m1, m2, m3, m4, m5);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 6>) noexcept
        {
            auto &[m1, m2, m3, m4, m5, m6] = t;

            return std::forward_as_tuple(m1, m2, m3, m4, m5, m6);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 7>) noexcept
        {
            auto &[m1, m2, m3, m4, m5, m6, m7] = t;

            return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7);
        }

        template <typename T>
        constexpr auto as_tuple(T &t, std::integral_constant<size_t, 8>) noexcept
        {
            auto &[m1, m2, m3, m4, m5, m6, m7, m8] = t;

            return std::forward_as_tuple(m1, m2, m3, m4, m5, m6, m7, m8);
        }
    }

    template <typename T>
    constexpr auto as_tuple(T &t) noexcept
    {
        return details::as_tuple(t, struct_arity<std::decay_t<T>>{});
    }
}

#endif /* !STRUCT_UTILS_AS_TUPLE_HPP */
