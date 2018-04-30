cmake_minimum_required(VERSION 3.8)
project(delta_ptr_proj)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(delta_ptr_SRC
        main.cpp
        delta_ptr.hpp)

add_executable(delta_ptr ${delta_ptr_SRC})
