cmake_minimum_required(VERSION 3.10)

project(JsonParser VERSION 1.0 DESCRIPTION "JSON Parser")

# cmake flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS};--output-on-failure;--force-new-ctest-process;--verbose")

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# no cmake in the main project directory
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "dumbass. lol")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Build type set to default")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# output binaries to a subdirectory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable CMake testing
include(CTest)
enable_testing()

# enable GoogleTest
include(GoogleTest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest googletest)
set(GTEST_OUTPUT "all")

# Enable Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS
log
log_setup
)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost lib: ${Boost_LIBRARY_DIRS}")

# ===== DIRECTORIES =====

# Add directory containing header files
include_directories(include)

# ===== LIBRARIES =====

# function to create a library
function(create_lib src_name src_dir)
  add_library(${src_name} STATIC ${src_dir}/${src_name}.cc)
  target_include_directories(${src_name} PUBLIC ${src_dir})
  target_link_libraries(${src_name} PRIVATE Boost::log Boost::log_setup)
endfunction()

# node libraries
create_lib(array_node "src/nodes")
create_lib(boolean_node "src/nodes")
create_lib(null_node "src/nodes")
create_lib(number_node "src/nodes")
create_lib(object_node "src/nodes")
create_lib(string_node "src/nodes")

# parse libraries
create_lib(tokenizer "src/parse")

# util libraries
create_lib(logger "src/utils")

# link the logger library to the necessary targets
target_link_libraries(tokenizer PRIVATE logger)

# ===== EXECUTABLES =====

# function to create an executable test
function(create_test_executable test_name test_dir)
  add_executable(${test_name} ${test_dir}/${test_name}.cc)
  target_link_libraries(${test_name} PRIVATE gtest_main Boost::log Boost::log_setup ${ARGN})
  gtest_discover_tests(${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${test_dir}
    PROPERTIES ENVIRONMENT "GTEST_COLOR=1"
  )
endfunction()

# Create test executables
create_test_executable(tokenizer_test "tests/parse" tokenizer)

# Link the logger library to the test executable
target_link_libraries(tokenizer_test PRIVATE logger)

# ===== CODE COVERAGE =====

include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(
  TARGETS
  array_node_lib
  boolean_node_lib
  null_node_lib
  number_node_lib
  object_node_lib
  string_node_lib
  tokenizer_lib

  TESTS
  tokenizer_test
)
