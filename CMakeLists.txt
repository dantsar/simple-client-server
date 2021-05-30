cmake_minimum_required(VERSION 3.13)
project("simple-client-server")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)

add_compile_options(-ggdb -Wall -pedantic)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

add_executable(client ${SOURCES})