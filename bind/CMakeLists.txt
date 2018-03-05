cmake_minimum_required(VERSION 3.8)
project(test_bind)

set(CMAKE_CXX_STANDARD 98)

if (UNIX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-ignored-attributes")
elseif (WIN32)
    set(CMAKE_CXX_FLAGS "/Ox /MP /std:c++03")
endif ()

set(step_SRC
        TypeTraits.hpp
        TypeList.hpp
        Storage.hpp
        MemberFunc.hpp
        Caller.hpp
        Function.hpp
        ut.h
        main.cpp
        )

add_executable(${PROJECT_NAME} ${step_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})