cmake_minimum_required(VERSION 3.12)
project(decimal_for_cpp)
include(CTest)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard version to use (default is 11)")
#set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TESTING)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS} include)
        add_executable(test_runner include/decimal.h tests/runner.cpp)
        target_include_directories(test_runner PRIVATE ${BOOST_INCLUDE_DIRS})
    endif()

    enable_testing()
    add_test(test_runner test_runner)
endif()

install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")