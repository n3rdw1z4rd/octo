cmake_minimum_required(VERSION 3.14.0)
project(octo VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -O2")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCES})

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$ENV{MINGW_ROOT}\\include"
        "$ENV{VULKAN_SDK}\\Include"
        "$ENV{SDKS_ROOT}\\glfw\\include"
        "$ENV{SDKS_ROOT}\\glm\\glm"
        "$ENV{SDKS_ROOT}\\spdlog\\include"
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{MINGW_ROOT}\\lib"
        "$ENV{VULKAN_SDK}\\Lib"
        "$ENV{SDKS_ROOT}\\glfw\\lib-mingw-w64"
    )

    target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${VULKAN_SDK}/include"
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        "${VULKAN_SDK}/lib"
    )

    target_link_libraries(${PROJECT_NAME} glfw vulkan)
endif()


