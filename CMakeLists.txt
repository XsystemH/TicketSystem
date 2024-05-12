cmake_minimum_required(VERSION 3.22)
project(TicketSystem)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O2) # 开启O2优化
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp
        BPT.h
        vector/exceptions.hpp
        vector/vector.hpp)
