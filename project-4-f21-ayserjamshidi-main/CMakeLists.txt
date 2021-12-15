cmake_minimum_required(VERSION 3.20)
project(project_4_f21_ayserjamshidi)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
        main

        src/main.cpp
        src/signalhandler/initialize.cpp
        src/signalhandler/logger.cpp
        src/helpers/generics.cpp
        src/signalhandler/receiver.cpp
        src/signalhandler/generator.cpp
        src/includes/errors.h
)

target_link_libraries(main PRIVATE Threads::Threads)