cmake_minimum_required(VERSION 3.24)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall)

# Set default compile flags for GCC
set(GOOD_COMPILERS Clang GNU)
if (CMAKE_CXX_COMPILER_ID IN_LIST GOOD_COMPILERS)
    message(STATUS "GCC detected, adding compile flags")
    add_compile_options(-Wextra -Werror -Wpedantic -pedantic-errors)
endif ()

set(SOURCE_DIR ./src)
set(SOURCE_FILES
        ${SOURCE_DIR}/package.cpp
        ${SOURCE_DIR}/storage_types.cpp
        ${SOURCE_DIR}/helpers.cpp
        ${SOURCE_DIR}/nodes.cpp
        ${SOURCE_DIR}/factory.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

include_directories(
        src
        include
)

include(GTestSetup.cmake)
