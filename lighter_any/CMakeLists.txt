cmake_minimum_required(VERSION 3.8)
project(lighter_any)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(lighter_any_SRC
        main.cpp
        lighter_any.hpp)

add_executable(lighter_any_demo ${lighter_any_SRC})
