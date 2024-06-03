cmake_minimum_required(VERSION 3.22)
project(TicketSystem)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O3) # 开启O2优化
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(code src/main.cpp
        src/BPT.h
        src/vector/exceptions.hpp
        src/vector/vector.hpp
        src/CommandProcesser.h
        src/Structs.h
        src/Storage.h
        src/map/map.hpp
        src/Core.h
        src/MergeSort.h
        src/LinearStorage.h
)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fsanitize=undefined,address" HAS_SANITIZERS)
if(HAS_SANITIZERS)
    target_compile_options(my_executable PRIVATE "-fsanitize=undefined,address")
    target_link_libraries(my_executable "-fsanitize=undefined,address")
else()
    message(WARNING "Compiler does not support sanitizers")
endif()
