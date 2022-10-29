cmake_minimum_required(VERSION 3.19)
project(octo VERSION 0.1.0 LANGUAGES CXX)

# Modules
include(FetchContent)

# Definitions
add_definitions(-DOCTO_PROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DOCTO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DOCTO_VERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DOCTO_VERSION_PATCH=${PROJECT_VERSION_PATCH})

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

# GLM
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

# FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
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
    ${PROJECT_SOURCE_DIR}/include
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