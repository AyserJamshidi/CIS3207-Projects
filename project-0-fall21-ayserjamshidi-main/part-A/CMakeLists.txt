cmake_minimum_required(VERSION 3.10)
project(part_a)

set(CMAKE_CXX_STANDARD 98)

configure_file(
        ex1.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(
        ex2.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(
        ex3.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_executable(
        Part-A-wcat
        wcat.cpp
)

add_executable(
        Part-A-wgrep
        wgrep.cpp
)