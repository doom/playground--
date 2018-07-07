cmake_minimum_required(VERSION 3.8)
project(constexpr_map_proj)

#set(CMAKE_CXX_COMPILER "clang++-6.0")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(constexpr_map_SRC
        main.cpp
        details.hpp
        set.hpp
        map.hpp
        )

add_executable(constexpr_map ${constexpr_map_SRC})
