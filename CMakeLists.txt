cmake_minimum_required(VERSION 3.24)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Werror -Wpedantic -pedantic-errors)

IF (!WIN_32)
    add_compile_options(-Wextra)
ENDIF ()

set(SOURCE_DIR ./src)
set(SOURCE_FILES
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

include_directories(src)

include(GTestSetup.cmake)
