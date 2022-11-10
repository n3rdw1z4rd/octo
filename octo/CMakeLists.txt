cmake_minimum_required(VERSION 3.24)
project(octo VERSION 0.1.0 LANGUAGES CXX)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DVERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${PROJECT_VERSION_PATCH})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "C:\\VulkanSDK\\1.3.231.1\\Include"
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC
        "C:\\VulkanSDK\\1.3.231.1\\Lib"
    )

    target_link_libraries(${PROJECT_NAME} vulkan-1 SDL2)
elseif (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # target_link_libraries(${PROJECT_NAME})
endif()