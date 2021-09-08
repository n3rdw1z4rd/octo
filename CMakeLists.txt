cmake_minimum_required(VERSION 3.21.0)
project(octo VERSION 0.1.0)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$ENV{MINGW_ROOT}\\include"
        "$ENV{VULKAN_SDK}\\Include"
        "$ENV{SDKS_ROOT}\\glfw\\include"
        "$ENV{SDKS_ROOT}\\glm\\glm"
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{MINGW_ROOT}\\lib"
        "$ENV{VULKAN_SDK}\\Lib"
        "$ENV{SDKS_ROOT}\\glfw\\lib-mingw-w64"
    )

    target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} glfw vulkan)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
