cmake_minimum_required(VERSION 3.22.0)
project(octo VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DOCTO_PRODUCT_NAME="${PROJECT_NAME}"
    -DOCTO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DOCTO_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DOCTO_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    -DNOMINMAX
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("==> Building for: LINUX")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DOCTO_PLATFORM_LINUX)
    target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} glfw vulkan)# vulkan shaderc_shared)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("==> Building for: MACOS")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DOCTO_PLATFORM_MACOS)
    target_include_directories(${PROJECT_NAME} PUBLIC /opt/homebrew/include)
    target_link_directories(${PROJECT_NAME} PUBLIC /opt/homebrew/lib)
    target_link_libraries(${PROJECT_NAME} glfw)# vulkan shaderc_shared)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("==> Building for: WINDOWS")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DOCTO_PLATFORM_WINDOWS)
endif()
