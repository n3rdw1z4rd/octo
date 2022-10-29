cmake_minimum_required(VERSION 3.19)
project(octo VERSION 0.1.0 LANGUAGES CXX)

# Modules
include(FetchContent)

# Definitions
add_definitions(-DENGINE_NAME="${PROJECT_NAME}")
add_definitions(-DVERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${PROJECT_VERSION_PATCH})

# GLFW
FetchContent_Declare(
    glfw
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

# Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Compilation
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Includes
target_include_directories(${PROJECT_NAME} PUBLIC
    ${glfw_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/include
)

# Libs
if (WIN32)
    target_link_directories(${PROJECT_NAME} PUBLIC
        ${glfw_BINARY_DIR}/src/Debug
    )

    target_link_libraries(${PROJECT_NAME} glfw3)
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} glfw)
endif()