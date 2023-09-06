cmake_minimum_required(VERSION 3.20.0)
project(octo VERSION 0.1.0 LANGUAGES CXX)

# include(FetchContent)

# FetchContent_Declare(glfw
#     GIT_REPOSITORY git@github.com:glfw/glfw.git
#     GIT_TAG 3.3.8
# )

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# FetchContent_MakeAvailable(glfw)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DENGINE_NAME="${PROJECT_NAME}"
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DNOMINMAX
)

if (WIN32)
    set(WIN32_VULKAN_SDK_ROOT "C:\\VulkanSDK\\1.3.231.1")

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        -DVK_USE_PLATFORM_WIN32_KHR
    )

    message(${WIN32_VULKAN_SDK_ROOT}/Include)
    
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${glfw_SOURCE_DIR}/include
        ${WIN32_VULKAN_SDK_ROOT}/Include
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        ${WIN32_VULKAN_SDK_ROOT}/Lib
        ${glfw_BINARY_DIR}/src/Debug
    )

    target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 shaderc_shared)
elseif (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        # $ENV{HOME}/dev/VulkanSDK/1.3.231.1/macos/include
        # ${glfw_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        $ENV{HOME}/dev/VulkanSDK/1.3.231.1/macos/lib
        # ${glfw_BINARY_DIR}/src/Debug
    )

    target_link_libraries(${PROJECT_NAME} glfw vulkan shaderc_shared)
endif()
