cmake_minimum_required(VERSION 3.10)
project(project2_sort_bench)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -O2)

add_executable(project2
    main.cpp
)

