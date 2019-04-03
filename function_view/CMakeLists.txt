cmake_minimum_required(VERSION 3.9)

project(function_view)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

set(FUNCTION_VIEW_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/function_view.hpp
        )

add_library(function_view INTERFACE)

target_sources(function_view INTERFACE ${FUNCTION_VIEW_SOURCES})

target_include_directories(function_view INTERFACE include)

find_package(GTest REQUIRED)

add_executable(function_view-tests
        tests/function_view-tests.cpp
        )

target_link_libraries(function_view-tests function_view GTest::GTest GTest::Main)
