cmake_minimum_required(VERSION 3.24)
project(NetSim)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

set(SOURCE_DIR ./src)
set(SOURCE_FILES
        main.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

include_directories(src)
include(GTestSetup.cmake)