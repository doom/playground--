//
// Created by doom on 01/07/18.
//

#include <cassert>
#include <memory>
#include <iostream>
#include "lighter_any.hpp"

int main()
{
    using any8 = doom::lighter_any<8, 8>;

    any8 a;

    static_assert(sizeof(any8) == any8::max_size + sizeof(void *));

    assert(!a.has_value());
    assert(a.type() == typeid(void));

    any8 b(a);
    assert(!b.has_value());
    assert(b.type() == typeid(void));

    b.emplace<int>(2);
    assert(b.has_value());
    assert(b.type() == typeid(int));

    auto value = doom::any_cast<int>(b);
    assert(value == 2);

    auto &ref = doom::any_cast<int &>(b);
    assert(ref == 2);
    ref = 3;
    assert(doom::any_cast<int>(b) == 3);

    using any_str = doom::lighter_any<sizeof(std::string), alignof(std::string)>;

    any_str a2;
    a2.emplace<std::string>("salutaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    a2.emplace<std::string>("salutbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    a2.emplace<std::string>("hello");
    const auto &str_ref = doom::any_cast<const std::string &>(a2);
    assert(str_ref == "hello");

    a2 = b; //Copying from any's of different sizes : works only if destination::max_size >= source::max_size
    assert(a2.has_value());
    assert(a2.type() == typeid(int));

    value = doom::any_cast<int>(a2);
    assert(value == 3);

    b.reset();
    assert(!b.has_value());
    assert(b.type() == typeid(void));

    any8 a3(doom::in_place_type_t<int>{}, 123);
    assert(a3.has_value());
    assert(a3.type() == typeid(int));
    assert(doom::any_cast<int>(a3) == 123);
    return 0;
}
