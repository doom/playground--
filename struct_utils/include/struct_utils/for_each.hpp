/*
** Created by doom on 28/12/18.
*/

#ifndef STRUCT_UTILS_FOREACH_HPP
#define STRUCT_UTILS_FOREACH_HPP

#include <cstddef>
#include <utility>
#include <struct_utils/as_tuple.hpp>

namespace su
{
    namespace details
    {
        template <typename Tuple, typename Func, std::size_t ...Is>
        constexpr void tuple_for_each_helper(Tuple &tup, Func &&func, std::index_sequence<Is...>)
        {
            (func(std::get<Is>(tup)), ...);
        }

        template <typename Tuple, typename Func>
        constexpr void tuple_for_each(Tuple &tup, Func &&func)
        {
            tuple_for_each_helper(tup, std::forward<Func>(func), std::make_index_sequence<std::tuple_size_v<Tuple>>{});
        }
    }

    template <typename T, typename Func>
    constexpr void for_each(T &t, Func &&func)
    {
        auto tup = as_tuple(t);

        details::tuple_for_each(tup, std::forward<Func>(func));
    }
}

#endif /* !STRUCT_UTILS_FOREACH_HPP */
