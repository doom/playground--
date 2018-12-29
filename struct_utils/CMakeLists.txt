cmake_minimum_required(VERSION 3.9)

project(struct_utils)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-missing-field-initializers -O3")

set(AS_TUPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/struct_utils/traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/struct_utils/as_tuple.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/struct_utils/stream_operator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/struct_utils/for_each.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/struct_utils/struct_utils.hpp
        )

add_library(struct_utils INTERFACE)

target_sources(struct_utils INTERFACE ${AS_TUPLE_SOURCES})

target_include_directories(struct_utils INTERFACE include)

find_package(GTest REQUIRED)

add_executable(struct_utils-tests
        tests/struct_utils-tests.cpp
        )

target_link_libraries(struct_utils-tests struct_utils GTest::GTest GTest::Main)
