cmake_minimum_required(VERSION 3.10)
project(JSONParser VERSION 1.0 DESCRIPTION "JSON Parser")

# cmake flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Prevent in-source builds
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Set default build type to Debug
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/nodes/null_node.cc
)

# Header files
file(GLOB_RECURSE HEADERS include/*.h)

# Create library
add_library(json_lib STATIC ${SOURCES} ${HEADERS})

# For future use: uncomment and modify when you add test files
# enable_testing()
# find_package(GTest REQUIRED)
#
# add_executable(json_tests
#     tests/json/json_deserializer_test.cc
#     tests/json/json_parser_test.cc
#     tests/json/json_tokenizer_test.cc
# )
#
# target_link_libraries(json_tests
#     GTest::GTest
#     GTest::Main
#     json_lib
# )
#
# include(GoogleTest)
# gtest_discover_tests(json_tests)

# This is just to make sure all files are included in the project structure
add_custom_target(project_files SOURCES ${HEADERS} ${SOURCES})
