cmake_minimum_required(VERSION 3.9)

project(polymorphic_wrapper)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER "clang++-10")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

set(POLYMORPHIC_WRAPPER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/polymorphic_wrapper.hpp
        )

add_library(polymorphic_wrapper INTERFACE)

target_sources(polymorphic_wrapper INTERFACE ${POLYMORPHIC_WRAPPER_SOURCES})

target_include_directories(polymorphic_wrapper INTERFACE include)

find_package(GTest REQUIRED)

add_executable(polymorphic_wrapper-tests
        tests/polymorphic_wrapper-tests.cpp
        )

target_link_libraries(polymorphic_wrapper-tests polymorphic_wrapper GTest::GTest GTest::Main)
