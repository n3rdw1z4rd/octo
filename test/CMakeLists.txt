cmake_minimum_required(VERSION 3.24)
project(test VERSION 0.1.0 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../octo/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # target_link_directories(${PROJECT_NAME} PUBLIC
    #     "C:\\VulkanSDK\\1.3.231.1\\Lib"
    # )

    target_link_libraries(${PROJECT_NAME} octo)
elseif (UNIX)
    # target_include_directories(${PROJECT_NAME} PUBLIC
    #     ${CMAKE_CURRENT_SOURCE_DIR}/../octo/include
    #     ${CMAKE_CURRENT_SOURCE_DIR}/include
    # )

    # target_link_libraries(${PROJECT_NAME})
endif()