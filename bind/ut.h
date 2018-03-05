
#ifndef UT_H
#define UT_H

#include <cstdio>

#define STR2(x)     #x
#define STR(x)      STR2(x)
#define __UT_LINE   STR(__LINE__)

#define __ut_put(x, ...)            fprintf(stderr, x, ##__VA_ARGS__)

#define ut_start_test()             unsigned int __passed = 0;

#define ut_end_test()      __ut_put("%5u assertions passed    [SUCCESS]\n",    \
                                    __passed);

#define __ut_test_failed(msg)                                               \
    __ut_put("failure: " "%14s", __FILE__ ": "                              \
             __UT_LINE "\n");                                               \
    return 1;

#define __ut_test_passed()                                                  \
    __passed += 1;

#define __ut_test_failed_if(cond, errmsg)                                   \
    if (cond) {                                                             \
        __ut_test_failed(errmsg);                                           \
    } else {                                                                \
        __ut_test_passed();                                                 \
    }

#define __ut_assert_rel(a, rel, b, err)                                     \
    __ut_test_failed_if(!(a rel b), #a err #b);

#define ut_assert(x)                                                        \
    __ut_test_failed_if(!(x), #x " to be true");

#define ut_assert_false(x)                                                  \
    __ut_test_failed_if(x, #x " to be false");

#define ut_assert_eq(a, b)                                                  \
    __ut_assert_rel(a, ==, b, " to be equal to ")

#define ut_assert_streq(a, b)                                               \
    __ut_assert_rel(strcmp(a, b), ==, 0, " to be equal to ")

#define ut_assert_memeq(a, b, sz)                                           \
    __ut_assert_rel(memcmp(a, b, sz), ==, 0, " to be equal to ")

#define ut_assert_ne(a, b)                                                  \
    __ut_assert_rel(a, !=, b, " to be different from ")

#define ut_assert_gt(a, b)                                                  \
    __ut_assert_rel(a, >, b, " to be greater than ")

#define ut_assert_ge(a, b)                                                  \
    __ut_assert_rel(a, >=, b, " to be greater or equal than ")

#define ut_assert_lt(a, b)                                                  \
    __ut_assert_rel(a, <, b, " to be less than ")

#define ut_assert_le(a, b)                                                  \
    __ut_assert_rel(a, <=, b, " to be lesser than or equal to ")

#endif //UT_H
