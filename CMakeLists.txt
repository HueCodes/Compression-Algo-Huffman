cmake_minimum_required(VERSION 3.14)
project(HuffmanCpp
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Huffman coding compression algorithm implementation"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE/tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler-specific warnings (portable approach)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
elseif(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

# Create library target for the Huffman implementation
add_library(huffman_lib STATIC
    src/huffman.cpp
)
target_include_directories(huffman_lib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Main executable
add_executable(huffman src/main.cpp)
target_link_libraries(huffman PRIVATE huffman_lib)

# Testing
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
