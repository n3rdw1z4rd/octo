cmake_minimum_required(VERSION 3.24)
project(octo-test LANGUAGES CXX)


if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) #/Debug)
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(octo)
add_subdirectory(test)