
#ifndef CORE_PP_FOR_EACH_HPP
#define CORE_PP_FOR_EACH_HPP

#include <pp/pp_count_args.hpp>
#include <pp/pp_paste.hpp>
#include <pp/details/pp_for_each.hpp>

#define pp_for_each(fct, ...)                                               \
    __FOR_EACH_EXPAND(pp_paste(__FOR_EACH, pp_count_args(__VA_ARGS__))(fct, __VA_ARGS__))

#endif //CORE_PP_FOR_EACH_HPP
