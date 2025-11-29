cmake_minimum_required(VERSION 3.20)
project(ISP_Pipeline 
    VERSION 0.1.0
    DESCRIPTION "A Modern C++ ISP Pipeline Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    -Wall -Wextra -Wpedantic
    -Werror=return-type
    -Wshadow
    -Wconversion
)

add_library(isp_core STATIC
    src/image.cpp
    src/io.cpp
    src/rgb_image.cpp
    src/modules/blc.cpp
    src/modules/demosaic.cpp
    src/modules/awb.cpp
)

target_include_directories(isp_core 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(isp_main src/main.cpp)
target_link_libraries(isp_main PRIVATE isp_core)

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Test RAW generator tool
add_executable(generate_test_raw tools/generate_test_raw.cpp)