cmake_minimum_required(VERSION 3.12)
project(decimal_for_cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard version to use (default is 11)")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} include)
    add_executable(test_runner include/decimal.h tests/runner.cpp tests/decimalTest.ipp)
    target_include_directories(test_runner PRIVATE ${BOOST_INCLUDE_DIRS})
endif()

enable_testing()
add_test(test_runner test_runner)
