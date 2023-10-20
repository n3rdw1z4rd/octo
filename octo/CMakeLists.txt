cmake_minimum_required(VERSION 3.22)
project(octo VERSION 0.1.0 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DPRODUCT_NAME="${PROJECT_NAME}"
    -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DNOMINMAX
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME}
        glfw
        vulkan
        shaderc_shared
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_include_directories(${PROJECT_NAME} PUBLIC
        /opt/homebrew/include
        $ENV{HOME}/dev/VulkanSDK/1.3.261.1/macos/include
    )
    
    target_link_directories(${PROJECT_NAME} PUBLIC
        /opt/homebrew/lib
        $ENV{HOME}/dev/VulkanSDK/1.3.261.1/macos/lib
    )
    
    target_link_libraries(${PROJECT_NAME}
        glfw
        vulkan
        shaderc_shared
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
endif()