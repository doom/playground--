#include <cassert>
#include "set.hpp"
#include "map.hpp"

void test_set_int() noexcept
{
    static constexpr doom::set<int, 6> si = {0, 1, 2, 3, 4, 5};

    constexpr bool empty = si.empty();
    static_assert(!empty);

    constexpr auto size = si.size();
    static_assert(size == 6);

    constexpr auto b0 = si.contains(0);
    constexpr auto b1 = si.contains(1);
    constexpr auto b2 = si.contains(2);
    constexpr auto b3 = si.contains(3);
    constexpr auto b4 = si.contains(4);
    constexpr auto b5 = si.contains(5);
    constexpr auto b6 = si.contains(6);

    static_assert(b0);
    static_assert(b1);
    static_assert(b2);
    static_assert(b3);
    static_assert(b4);
    static_assert(b5);
    static_assert(!b6);

    constexpr auto i0 = si.find(0);
    constexpr auto i1 = si.find(1);
    constexpr auto i2 = si.find(2);
    constexpr auto i3 = si.find(3);
    constexpr auto i4 = si.find(4);
    constexpr auto i5 = si.find(5);
    constexpr auto i6 = si.find(6);

    static_assert(i0 != si.end());
    static_assert(i1 != si.end());
    static_assert(i2 != si.end());
    static_assert(i3 != si.end());
    static_assert(i4 != si.end());
    static_assert(i5 != si.end());
    static_assert(i6 == si.end());

    constexpr auto a0 = si.at(0);
    constexpr auto a1 = si.at(1);
    constexpr auto a2 = si.at(2);
    constexpr auto a3 = si.at(3);
    constexpr auto a4 = si.at(4);
    constexpr auto a5 = si.at(5);

    static_assert(a0 == 0);
    static_assert(a1 == 1);
    static_assert(a2 == 2);
    static_assert(a3 == 3);
    static_assert(a4 == 4);
    static_assert(a5 == 5);
}

void test_map_int() noexcept
{
    static constexpr doom::map<int, int, 5> mii = {
        {0, -0},
        {1, -1},
        {2, -2},
        {3, -3},
        {4, -4},
    };

    constexpr bool empty = mii.empty();
    static_assert(!empty);

    constexpr auto size = mii.size();
    static_assert(size == 5);

    constexpr auto b0 = mii.contains(0);
    constexpr auto b1 = mii.contains(1);
    constexpr auto b2 = mii.contains(2);
    constexpr auto b3 = mii.contains(3);
    constexpr auto b4 = mii.contains(4);
    constexpr auto b5 = mii.contains(5);

    static_assert(b0);
    static_assert(b1);
    static_assert(b2);
    static_assert(b3);
    static_assert(b4);
    static_assert(!b5);

    constexpr auto i0 = mii.find(0);
    constexpr auto i1 = mii.find(1);
    constexpr auto i2 = mii.find(2);
    constexpr auto i3 = mii.find(3);
    constexpr auto i4 = mii.find(4);
    constexpr auto i5 = mii.find(5);

    static_assert(i0 != mii.end());
    static_assert(i0->second == -0);
    static_assert(i1 != mii.end());
    static_assert(i1->second == -1);
    static_assert(i2 != mii.end());
    static_assert(i3 != mii.end());
    static_assert(i4 != mii.end());
    static_assert(i5 == mii.end());

    constexpr auto &a0 = mii.at(0);
    constexpr auto &a1 = mii.at(1);
    constexpr auto &a2 = mii.at(2);
    constexpr auto &a3 = mii.at(3);
    constexpr auto &a4 = mii.at(4);

    static_assert(a0 == -0);
    static_assert(a1 == -1);
    static_assert(a2 == -2);
    static_assert(a3 == -3);
    static_assert(a4 == -4);
}


void test_map_string() noexcept
{
    static constexpr doom::map<int, const char *, 5> mis = {
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"},
    };

    constexpr bool empty = mis.empty();
    static_assert(!empty);

    constexpr auto size = mis.size();
    static_assert(size == 5);

    constexpr auto b0 = mis.contains(0);
    constexpr auto b1 = mis.contains(1);
    constexpr auto b2 = mis.contains(2);
    constexpr auto b3 = mis.contains(3);
    constexpr auto b4 = mis.contains(4);
    constexpr auto b5 = mis.contains(5);

    static_assert(b0);
    static_assert(b1);
    static_assert(b2);
    static_assert(b3);
    static_assert(b4);
    static_assert(!b5);

    constexpr auto i0 = mis.find(0);
    constexpr auto i1 = mis.find(1);
    constexpr auto i2 = mis.find(2);
    constexpr auto i3 = mis.find(3);
    constexpr auto i4 = mis.find(4);
    constexpr auto i5 = mis.find(5);

    static_assert(i0 != mis.end());
    static_assert(std::string_view{"zero"} == i0->second);
    static_assert(i1 != mis.end());
    static_assert(std::string_view{"one"} == i1->second);
    static_assert(i2 != mis.end());
    static_assert(std::string_view{"two"} == i2->second);
    static_assert(i3 != mis.end());
    static_assert(std::string_view{"three"} == i3->second);
    static_assert(i4 != mis.end());
    static_assert(std::string_view{"four"} == i4->second);
    static_assert(i5 == mis.end());

    constexpr auto &a0 = mis.at(0);
    constexpr auto &a1 = mis.at(1);
    constexpr auto &a2 = mis.at(2);
    constexpr auto &a3 = mis.at(3);
    constexpr auto &a4 = mis.at(4);

    static_assert(std::string_view{"zero"} == a0);
    static_assert(std::string_view{"one"} == a1);
    static_assert(std::string_view{"two"} == a2);
    static_assert(std::string_view{"three"} == a3);
    static_assert(std::string_view{"four"} == a4);
}

void test_stuff() noexcept
{
    static constexpr doom::map<int, float, 3> mif{
        {1, 1.0},
        {2, 2.0},
        {3, 3.0},
    };

    constexpr auto empty = mif.empty();
    static_assert(!empty);

    constexpr auto size = mif.size();
    static_assert(size == 3);

    constexpr auto count = mif.count(3);
    static_assert(count == 1);

    constexpr auto find = mif.find(5);
    static_assert(find == mif.end());

    static_assert(mif.begin() != mif.end());

    static_assert(std::distance(mif.rbegin(), mif.rend()) == 3);
}

int main()
{
    test_set_int();
    test_map_int();
    test_map_string();
    test_stuff();

    return 0;
}
