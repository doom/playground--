/*
** Created by doom on 27/12/18.
*/

#ifndef STRUCT_UTILS_STREAM_OPERATOR_HPP
#define STRUCT_UTILS_STREAM_OPERATOR_HPP

#include <struct_utils/traits.hpp>
#include <struct_utils/as_tuple.hpp>

namespace su::stream_operator
{
    namespace details
    {
        template <typename Os, typename T, typename = void>
        struct is_streamable_helper : std::false_type
        {
        };

        template <typename Os, typename T>
        struct is_streamable_helper<Os, T, std::void_t<decltype(std::declval<Os &>() << std::declval<const T &>())>> :
            std::true_type
        {
        };

        template <typename Os, typename T>
        inline constexpr bool is_streamable_v = is_streamable_helper<Os, T>::value;
    }

    template <typename OutputStream, typename T>
    std::enable_if_t<std::is_aggregate_v<T> && !details::is_streamable_v<OutputStream, T>, OutputStream &>
    operator<<(OutputStream &os, const T &t);

    namespace details
    {
        template <typename OutputStream, typename Tuple, std::size_t ...Is>
        void print_tuple(OutputStream &os, const Tuple &t, std::index_sequence<Is...>)
        {
            os << "{";
            (((Is > 0 ? os << ", " : os) << std::get<Is>(t)), ...);
            os << "}";
        }
    }

    template <typename OutputStream, typename T>
    std::enable_if_t<std::is_aggregate_v<T> && !details::is_streamable_v<OutputStream, T>, OutputStream &>
    operator<<(OutputStream &os, const T &t)
    {
        details::print_tuple(os, as_tuple(t), std::make_index_sequence<su::struct_arity_v<T>>{});
        return os;
    }
}

#endif /* !STRUCT_UTILS_STREAM_OPERATOR_HPP */
