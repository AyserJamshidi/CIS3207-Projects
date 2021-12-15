cmake_minimum_required(VERSION 3.5)
project(project_2_fall21_ayserjamshidi)

set(CMAKE_CXX_STANDARD 11)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

configure_file(
        readme_doc
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(
        batch.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

configure_file(
        output.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

add_executable(
        Interactive
        main.cpp
        Helpers/redirection.cpp
        Helpers/redirection.h
        InternalCommands/dir.cpp Helpers/directory_helper.cpp Helpers/directory_helper.h InternalCommands/internal_commands.h InternalCommands/cd.cpp InternalCommands/clr.cpp InternalCommands/echo.cpp InternalCommands/pause.cpp InternalCommands/help.cpp InternalCommands/environ.cpp Structures/shell.h Helpers/vector_manipulation.cpp Helpers/vector_manipulation.h InternalCommands/path.cpp Helpers/file_helper.cpp Helpers/file_helper.h Helpers/print_helper.cpp Helpers/print_helper.h)

add_executable(
        Batch
        main.cpp
        Helpers/redirection.cpp
        Helpers/redirection.h
        InternalCommands/dir.cpp Helpers/directory_helper.cpp Helpers/directory_helper.h InternalCommands/internal_commands.h InternalCommands/cd.cpp InternalCommands/clr.cpp InternalCommands/echo.cpp InternalCommands/pause.cpp InternalCommands/help.cpp InternalCommands/environ.cpp Structures/shell.h Helpers/vector_manipulation.cpp Helpers/vector_manipulation.h InternalCommands/path.cpp Helpers/file_helper.cpp Helpers/file_helper.h Helpers/print_helper.cpp Helpers/print_helper.h)