cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)

add_compile_options(-ggdb -Wall -pedantic)

project("simple-client-server")

add_executable(client client.c)