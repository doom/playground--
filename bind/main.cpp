
#include <stdint.h>
#include <iostream>
#if 0
#include <functional>
#include <type_traits>
#endif
#include "ut.h"
#include "Storage.hpp"
#include "TypeTraits.hpp"
#include "TypeList.hpp"
#include "Caller.hpp"
#include "Function.hpp"

void setInt(int &r, int other)
{
    r = other;
}

int divInt(int a, int b)
{
    return a / b;
}

int delta(int a, int b, int c)
{
    return b * b - 4 * a * c;
}

int add5(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}

struct Foo
{
    int lul()
    {
        return 42;
    }

    int lala() const
    {
        return 42;
    }

    int bar(int)
    {
        return 84;
    }

    int baz(int) const
    {
        return 168;
    }
};

struct NonCopyable
{
public:
    NonCopyable(int n) : n(n)
    {
    }

private:
    NonCopyable(const NonCopyable &)
    {
    }

public:
    int n;
};

int getValueFromNonCopyable(const NonCopyable &nc, int other)
{
    return nc.n * other;
}

void setValueInNonCopyable(NonCopyable &nc, int toSet)
{
    nc.n = toSet;
}

struct Lol
{
    int operator()(int i)
    {
        return i;
    }
};

struct Lel : public Lol
{
    std::string func(const std::string &str)
    {
        return str;
    }
};

int main()
{
    ut_start_test();

    Storage1<double> s1(2.f);
    ut_assert_eq(s1._t1, 2.f);

    Storage2<int, int> s2(1, 2);
    ut_assert_eq(s2._t1, 1);
    ut_assert_eq(s2._t2, 2);

    Storage3<int, int, std::string> s3(3, 2, "aa");
    ut_assert_eq(s3._t1, 3);
    ut_assert_eq(s3._t2, 2);
    ut_assert_eq(s3._t3, "aa");

    Storage4<int, int, std::string, double> s4(3, 2, "zz", 2.f);
    ut_assert_eq(s4._t1, 3);
    ut_assert_eq(s4._t2, 2);
    ut_assert_eq(s4._t3, "zz");
    ut_assert_eq(s4._t4, 2.f);

    Storage5<int, int, std::string, double, unsigned int> s5(3, 2, "zz", 2.f, 2u);
    ut_assert_eq(s5._t1, 3);
    ut_assert_eq(s5._t2, 2);
    ut_assert_eq(s5._t3, "zz");
    ut_assert_eq(s5._t4, 2.f);
    ut_assert_eq(s5._t5, 2u);

    Storage6<int, int, std::string, double, unsigned int, char> s6(3, 2, "zz", 2.f, 2u, 'c');
    ut_assert_eq(s6._t1, 3);
    ut_assert_eq(s6._t2, 2);
    ut_assert_eq(s6._t3, "zz");
    ut_assert_eq(s6._t4, 2.f);
    ut_assert_eq(s6._t5, 2u);
    ut_assert_eq(s6._t6, 'c');

#if 0
    ut_assert((std::is_same<int, int32_t>::value == is_same<int, int32_t>::value));
    ut_assert((std::is_same<int, int64_t>::value == is_same <int, int64_t>::value));
    ut_assert((std::is_same<float, int32_t>::value == is_same<float, int32_t>::value));

    ut_assert((std::is_same<int, int>::value == is_same<int, int>::value));
    ut_assert((std::is_same<int, unsigned int>::value == is_same < int, unsigned int>::value));
    ut_assert((std::is_same<int, signed int>::value == is_same < int, signed int>::value));

    ut_assert((std::is_same<char, char>::value == is_same< char, char>::value));
    ut_assert((std::is_same<char, unsigned char>::value == is_same< char, unsigned char>::value));
    ut_assert((std::is_same<char, signed char>::value == is_same< char, signed char>::value));
#endif

    Value<int> v1(2);
    ut_assert_eq(v1.get(), 2);

    Value<std::string> v2("lalala");
    ut_assert_eq(v2.get(), "lalala");

    Function<int()> f = bind(delta, 1, 2, 1);
    ut_assert_eq(f(), 0);

    Function<int()> f2 = bind(add5, 1, 2, 3, 4, 5);
    ut_assert_eq(f2(), 15);

    Foo foo;
    Function<int()> f3 = bind(&Foo::lul, foo);
    ut_assert_eq(f3(), 42);

    Function<int()> f4 = bind(&Foo::bar, foo, 1);
    ut_assert_eq(f4(), 84);

    NonCopyable nc(42);
    Function<int()> f5 = bind(getValueFromNonCopyable, cref(nc), 2);
    ut_assert_eq(f5(), 84);

    Function<void()> f6 = bind(setValueInNonCopyable, ref(nc), 84);
    f6();
    ut_assert_eq(nc.n, 84);

    Function<int(int, int, int)> f7 = &delta;
    Function<int()> f8 = bind(f7, 1, 2, 1);
    ut_assert_eq(f8(), 0);

    Lol l;
    Function<int()> f9 = bind(&Lol::operator(), l, 1);
    ut_assert_eq(f9(), 1);

    Function<int()> f10 = bind(&Foo::baz, foo, 1);
    ut_assert_eq(f10(), 168);

    Lel ll;
    Function<int()> f11 = bind(&Lol::operator(), static_cast<Lol &>(ll), 1);
    ut_assert_eq(f11(), 1);

    Function<std::string()> f12 = bind(&Lel::func, ll, "salut");
    ut_assert_eq(f12(), "salut");

    Function<int()> f13 = bind(&Foo::lala, foo);
    ut_assert_eq(f13(), 42);

    Function<int(int)> f14 = bind(add5, _1, 2, 3, 4, 5);
    ut_assert_eq(f14(1), 15);

    Function<int(int, int)> f15 = bind(divInt, _2, _1);
    ut_assert_eq(f15(2, 4), 2);

    Function<int(int)> f16 = bind(add5, 1, 2, _1, 4, 5);
    ut_assert_eq(f16(3), 15);

    Function<int()> f17 = bind(bind(delta, 1, 2, _1), 1);
    ut_assert_eq(f17(), 0);

    int i = 3;
    Function<void()> f18 = bind(setInt, ref(i), 2);
    f18();
    ut_assert_eq(i, 2);

    ut_end_test();
}
