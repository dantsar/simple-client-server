cmake_minimum_required(VERSION 3.13)
project("simple-client-server")

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)

add_compile_options(-ggdb -Wall -pedantic)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

# sources of all the shared files
file(GLOB SHARED_SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

# sources for the client
file(GLOB CLIENT_SOURCES ${CMAKE_SOURCE_DIR}/src/client/*.c)

# sources for the server
file(GLOB SERVER_SOURCES ${CMAKE_SOURCE_DIR}/src/server/*.c)

add_executable(client ${SHARED_SOURCES} ${CLIENT_SOURCES})
add_executable(server ${SHARED_SOURCES} ${SERVER_SOURCES})
