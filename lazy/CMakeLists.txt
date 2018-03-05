cmake_minimum_required(VERSION 3.8)
project(lazy_proj)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(lazy_SRC
        main.cpp
        )

add_executable(lazy ${lazy_SRC})
