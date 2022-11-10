cmake_minimum_required(VERSION 3.24)
project(octo VERSION 0.1.0 LANGUAGES CXX)

add_definitions(-DOCTO_PROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DOCTO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DOCTO_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DOCTO_VERSION_PATCH=${PROJECT_VERSION_PATCH})

include(FetchContent)

# GLFW
FetchContent_Declare(glfw
    GIT_REPOSITORY git@github.com:glfw/glfw.git
    GIT_TAG 3.3.8
)

if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${glfw_SOURCE_DIR}/include
        "C:\\VulkanSDK\\1.3.231.1\\Include"
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        ${glfw_BINARY_DIR}/src/Debug
        "C:\\VulkanSDK\\1.3.231.1\\Lib"
    )

    target_link_libraries(${PROJECT_NAME} vulkan-1 glfw3)
elseif (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${glfw_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # target_link_libraries(${PROJECT_NAME})
endif()