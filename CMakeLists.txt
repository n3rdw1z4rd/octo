cmake_minimum_required(VERSION 3.20.0)
project(octo VERSION 0.1.0)

add_definitions(-DENGINE_NAME="${PROJECT_NAME}")
add_definitions(-DVERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${PROJECT_VERSION_PATCH})
add_definitions(-DNOMINMAX)

if (WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
endif()

include(FetchContent)

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

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "C:\\VulkanSDK\\1.3.231.1\\Include"
        ${glfw_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        "C:\\VulkanSDK\\1.3.231.1\\Lib"
        ${glfw_BINARY_DIR}/src/Debug
    )

    target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 shaderc_shared)
elseif (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        $ENV{HOME}/dev/VulkanSDK/1.3.231.1/macos/include
        ${glfw_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        $ENV{HOME}/dev/VulkanSDK/1.3.231.1/macos/lib
        ${glfw_BINARY_DIR}/src/Debug
    )

    target_link_libraries(${PROJECT_NAME} glfw vulkan shaderc_shared)
endif()