/*
** Created by doom on 14/12/18.
*/

#include <gtest/gtest.h>
#include <type_traits>
#include <struct_utils/struct_utils.hpp>

namespace
{
    struct two_ints
    {
        int a;
        int b;
    };

    struct one_int
    {
        int a;
    };

    struct two_ints_and_a_char
    {
        two_ints ti;
        char c;
    };

    struct empty
    {
    };

    inline std::ostream &operator<<(std::ostream &os, const one_int &oi)
    {
        return os << "one_int: a=" << oi.a;
    }
}

TEST(struct_utils, struct_arity)
{
    static_assert(su::struct_arity_v<two_ints> == 2);
    static_assert(su::struct_arity_v<one_int> == 1);
    static_assert(su::struct_arity_v<two_ints_and_a_char> == 2);
    static_assert(su::struct_arity_v<empty> == 0);
}

TEST(struct_utils, as_tuple)
{
    two_ints ti{1, 2};
    const two_ints ti2{3, 4};

    auto t = su::as_tuple(ti);
    static_assert(std::is_same_v<decltype(t), std::tuple<int &, int &>>);
    auto t2 = su::as_tuple(ti2);
    static_assert(std::is_same_v<decltype(t2), std::tuple<const int &, const int &>>);
}

TEST(struct_utils, stream_operator)
{
    using namespace su::stream_operator;
    std::ostringstream oss;
    two_ints ti{1, 2};
    one_int oi{3};
    two_ints_and_a_char tic{{4, 5}, 'a'};
    empty e;

    oss << ti;
    ASSERT_EQ(oss.str(), "{1, 2}");

    oss.str("");
    oss << oi;
    ASSERT_EQ(oss.str(), "one_int: a=3");

    oss.str("");
    oss << tic;
    ASSERT_EQ(oss.str(), "{{4, 5}, a}");

    oss.str("");
    oss << e;
    ASSERT_EQ(oss.str(), "{}");
}

namespace
{
    template <typename ...Ts>
    struct overload : private Ts ...
    {
        template <typename ...Us>
        constexpr overload(Us &&...us) : Ts{std::forward<Us>(us)}...
        {
        }

        using Ts::operator()...;
    };

    template <typename ...Ts>
    constexpr auto make_overload(Ts &&...ts) noexcept
    {
        return overload<Ts...>{std::forward<Ts>(ts)...};
    }
}

TEST(struct_utils, for_each)
{
    const two_ints_and_a_char tic{{1, 2}, 'a'};
    const char *pc = nullptr;
    const two_ints *pti = nullptr;

    su::for_each(tic, make_overload([&pti](const two_ints &ti) {
        ASSERT_EQ(pti, nullptr);
        pti = &ti;
    }, [&pc](const char &c) {
        ASSERT_EQ(pc, nullptr);
        pc = &c;
    }));
    ASSERT_EQ(pc, &tic.c);
    ASSERT_EQ(pti, &tic.ti);

    two_ints ti2{0, 1};
    su::for_each(ti2, make_overload([](auto &&i) {
        static_assert(std::is_same_v<decltype(i), int &>);
    }));

    const two_ints ti3{0, 1};
    su::for_each(ti3, make_overload([](auto &&i) {
        static_assert(std::is_same_v<decltype(i), const int &>);
    }));
}
