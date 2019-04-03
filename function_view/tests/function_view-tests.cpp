/*
** Created by doom on 14/12/18.
*/

#include <gtest/gtest.h>
#include "function_view.hpp"

static int add(int a, int b)
{
    return a + b;
}

class non_const_callable_add
{
public:
    int operator()(int a, int b)
    {
        return a + b;
    }
};

class only_const_callable_add
{
public:
    int operator()(int a, int b) const
    {
        return a + b;
    }
};

TEST(function_view, basic)
{
    function_view<int(int, int)> f = [](int a, int b) { return a + b; };
    ASSERT_EQ(f(1, 2), 3);

    function_view<int(int, int)> f2 = add;
    ASSERT_EQ(f2(1, 2), 3);

    non_const_callable_add ncca;
    function_view<int(int, int)> f3 = ncca;
    ASSERT_EQ(f3(1, 2), 3);

    only_const_callable_add occa;
    function_view<int(int, int)> f4 = occa;
    ASSERT_EQ(f4(1, 2), 3);
}
