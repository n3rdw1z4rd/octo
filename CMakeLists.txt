cmake_minimum_required(VERSION 3.22)
project(octo_engine)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED On)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(test)
add_subdirectory(octo)
