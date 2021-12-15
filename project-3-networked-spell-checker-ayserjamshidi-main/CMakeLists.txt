cmake_minimum_required(VERSION 3.20)
project(project_3_networked_spell_checker_ayserjamshidi)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(.)

configure_file(
        words
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_executable(
        Server

        src/server.cpp
        src/Helpers/open_listener_socket.h
        src/dictionary.cpp
        src/Helpers/map_manipulation.cpp
        src/Helpers/generics.cpp
        src/Helpers/printers.cpp
        src/Headers/server_structures.h
)

target_link_libraries(Server PRIVATE Threads::Threads)
