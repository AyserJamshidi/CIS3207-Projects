cmake_minimum_required(VERSION 3.5)
project(project_1_fall21_ayserjamshidi)

set(CMAKE_CXX_STANDARD 11)

configure_file(
        config.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_executable(
        main
        main.cpp
        config_file.cpp
        config_file.h
        model/job_event.h
        model/job.cpp
        model/job.h
)